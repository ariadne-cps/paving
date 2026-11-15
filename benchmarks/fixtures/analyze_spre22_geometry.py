#!/usr/bin/env python3
"""Static, exact-rational Bernstein certificates for the real SPRE22 fixture.

This is not a Paver run, a numerical optimizer, or a performance benchmark.
Only Python's standard library is used. Binary64 coefficients and uniform
errors are converted to exact Fractions before any polynomial manipulation.

The normalized Taylor variables are in [-1,1]. We transform each polynomial
to its tensor-product Bernstein form on [0,1]. The minimum Bernstein
coefficient minus the uniform error is a mathematically valid lower bound
for every function represented by the validated Taylor model.
"""

import argparse
from dataclasses import dataclass
from fractions import Fraction
from functools import lru_cache
from itertools import product
from math import comb, isfinite
from pathlib import Path
import re
import struct

BITS = re.compile(r"0x([0-9a-fA-F]+)ULL")
TERM = re.compile(r"tm\[MultiIndex\{([^}]*)\}\]\s*=\s*(.*?);")
ERROR = re.compile(r"tm\.set_error\((.*?)\);")
DOMAIN = re.compile(r"domain\[\du\]\s*=\s*ExactIntervalType\((.*?)\);")


def exact_float(bits: str) -> Fraction:
    x = struct.unpack(">d", bytes.fromhex(bits.zfill(16)))[0]
    if not isfinite(x):
        raise ValueError("Non-finite value in Taylor coefficient or domain endpoint")
    return Fraction.from_float(x)


@dataclass
class Model:
    terms: dict[tuple[int, ...], Fraction]
    error: Fraction

    def naive_lower(self) -> Fraction:
        zero = (0,) * 6
        return self.terms.get(zero, Fraction(0)) - sum(
            (abs(c) for powers, c in self.terms.items() if powers != zero),
            Fraction(0),
        ) - self.error


@dataclass
class Enclosure:
    domain: list[tuple[Fraction, Fraction]]
    models: list[Model]


def parse_fixture(path: Path) -> list[Enclosure]:
    text = path.read_text()
    if "Generated from real SPRE22 evolver enclosures" not in text:
        raise ValueError("File is not the expected real SPRE22 capture")
    entries = text.split("        sets.emplace_back(")
    if len(entries) != 49:
        raise ValueError(f"Expected 48 enclosures, found {len(entries)-1}")
    enclosures = []
    for number, block in enumerate(entries[:-1], 1):
        domain_source = block.rsplit("        ExactBoxType domain(6u);", 1)[1]
        domain_source = domain_source.split("        Vector<ValidatedTaylorModelDP>", 1)[0]
        domain = []
        for match in DOMAIN.findall(domain_source):
            pair = BITS.findall(match)
            if len(pair) != 2:
                raise ValueError(f"Malformed domain endpoint in enclosure {number}")
            domain.append((exact_float(pair[0]), exact_float(pair[1])))

        models = []
        for model_block in block.split("            ValidatedTaylorModelDP tm(")[1:]:
            model_block = model_block.split("        }", 1)[0]
            coefficients = {}
            for indexes, expression in TERM.findall(model_block):
                multi_index = tuple(int(p.strip().removesuffix("u")) for p in indexes.split(","))
                coefficient = BITS.findall(expression)
                if len(coefficient) != 1 or len(multi_index) != 6 or multi_index in coefficients:
                    raise ValueError(f"Malformed Taylor term in enclosure {number}")
                coefficients[multi_index] = exact_float(coefficient[0])
            matches = ERROR.findall(model_block)
            if len(matches) != 1 or len(BITS.findall(matches[0])) != 1:
                raise ValueError(f"Malformed Taylor error in enclosure {number}")
            error = exact_float(BITS.findall(matches[0])[0])
            if error < 0:
                raise ValueError(f"Negative model error in enclosure {number}")
            models.append(Model(coefficients, error))
        if len(domain) != 6 or len(models) != 6 or block.count("constraints.append(") != 1:
            raise ValueError(f"Unexpected model/constraint structure in enclosure {number}")
        if domain[0] != (Fraction(0), Fraction(0)):
            raise ValueError("Unexpected first parameter in the capture")
        if any(powers[0] != 0 for m in models for powers in m.terms):
            raise ValueError("Taylor model uses the first, fixed parameter")
        enclosures.append(Enclosure(domain, models))
    return enclosures


@lru_cache(None)
def monomial_bernstein_weight(exponent: int, degree: int, index: int) -> Fraction:
    """Bernstein coefficient of (2*u-1)^exponent at basis index."""
    return sum(
        (Fraction(comb(exponent, k) * 2**k * (-1)**(exponent-k)
                  * comb(index, k), comb(degree, k))
         for k in range(min(exponent, index)+1)),
        Fraction(0),
    )


def certified_bernstein_lower(model: Model) -> Fraction:
    """Exact bound: p(u) >= min_i b_i, and F(u) >= p(u)-error."""
    axes = (1, 2, 3, 4, 5)
    degrees = tuple(max(powers[j] for powers in model.terms) for j in axes)
    minimum = None
    for basis_index in product(*(range(d+1) for d in degrees)):
        coefficient = Fraction(0)
        for exponents, value in model.terms.items():
            weight = Fraction(1)
            for j, degree, index in zip(axes, degrees, basis_index):
                weight *= monomial_bernstein_weight(exponents[j], degree, index)
            coefficient += value * weight
        if minimum is None or coefficient < minimum:
            minimum = coefficient
    return minimum - model.error


def group_bounds(enclosures, first, last):
    results = {}
    for name, axis in (("x", 1), ("y", 2)):
        naive_margins, certified_margins = [], []
        for enclosure in enclosures[first:last]:
            face = enclosure.domain[axis][0]
            naive_margins.append(enclosure.models[axis].naive_lower()-face)
            certified_margins.append(certified_bernstein_lower(enclosure.models[axis])-face)
        results[name] = {
            "naive_min": min(naive_margins),
            "naive_max": max(naive_margins),
            "cert_min": min(certified_margins),
            "cert_max": max(certified_margins),
        }
    return results


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--fixture", type=Path,
                        default=Path(__file__).with_name("spre22_enclosures.generated.hpp"))
    parser.add_argument("--full", action="store_true", help="show per-enclosure values")
    args = parser.parse_args()
    enclosures = parse_fixture(args.fixture)
    print("SPRE22_STATIC_CERTIFICATE count=48 exact_fraction=true reference_grid=(1,200,200,20,20) fineness=4")
    for group in range(3):
        result = group_bounds(enclosures, 16*group, 16*(group+1))
        for axis in ("x", "y"):
            v = result[axis]
            print(f"group={group+1} enclosures={16*group+1}-{16*(group+1)} axis={axis} "
                  f"naive_margin=[{float(v['naive_min']):+.12f},{float(v['naive_max']):+.12f}] "
                  f"bernstein_margin=[{float(v['cert_min']):+.12f},{float(v['cert_max']):+.12f}] "
                  f"proves_separation={v['cert_min'] > 0}")
    for i in (0, 16):
        e = enclosures[i]
        print(f"example_enclosure={i+1} domain_x_lower={float(e.domain[1][0]):.6f} "
              f"simple_x_lower={float(e.models[1].naive_lower()):.12f} "
              f"certified_x_lower={float(certified_bernstein_lower(e.models[1])):.12f} "
              f"simple_y_lower={float(e.models[2].naive_lower()):.12f} "
              f"certified_y_lower={float(certified_bernstein_lower(e.models[2])):.12f}")
    if args.full:
        for i, e in enumerate(enclosures):
            print(f"enclosure={i+1} " + " ".join(
                f"{axis}_margin={float(certified_bernstein_lower(e.models[j])-e.domain[j][0]):+.12f}"
                for axis,j in (("x",1),("y",2))))


if __name__ == "__main__":
    main()
