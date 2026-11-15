# Captured SPRE22 enclosure fixture

The file `spre22_enclosures.generated.hpp` is produced from the **real**
SPRE22 evolver output, not hand-authored or synthesized. It was initially
absent; the real 48-enclosure capture has now been committed to Paving.

From `ariadne-cps/arch` on `spre22-paver-benchmark`, after building
`benchmark_spre22_pavers`, run:

```sh
./build-spre22/suite/nln/benchmark_spre22_pavers \
  --horizon-denominator 16 --max-jumps 1 \
  --max-enclosures 48 --finenesses 4 \
  --export-fixture /absolute/path/to/paving/benchmarks/fixtures/spre22_enclosures.generated.hpp
```

Then reconfigure the standalone Paving Release build; the target
`benchmark_spre22_fixture` is registered only with the generated
header present. Run it with `--fineness 4 --samples 5
--max-enclosures 48 --hybrid-switch 16`.

Before committing the fixture, check that there are exactly 48 real
enclosures and that the standalone replay matches ARCH's original
cell counts and volumes and reproduces the exact geometric containment
between the three algorithms. Count and volume agreement alone is not
proof of exact cross-repository geometric equality. Preserve the
generator commit and capture parameters in the fixture commit message.

No production Paving code or gitlinks depend on this fixture.

## Static geometric analysis

The results of the exact-rational Bernstein inspection of the real
fixture, including certified margins and limitations, are recorded
in `doc/paver_design.md`. The one-off Python script was intentionally
removed; any future automated analysis will use C++.
