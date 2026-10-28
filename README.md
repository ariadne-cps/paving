# Ariadne Paving

[![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![Unix Status](https://github.com/ariadne-cps/paving/actions/workflows/unix.yml/badge.svg)](https://github.com/ariadne-cps/paving/actions/workflows/unix.yml)
[![Windows Status](https://github.com/ariadne-cps/paving/actions/workflows/win.yml/badge.svg)](https://github.com/ariadne-cps/paving/actions/workflows/win.yml)
[![Sanitizers Status](https://github.com/ariadne-cps/paving/actions/workflows/sanitizers.yml/badge.svg)](https://github.com/ariadne-cps/paving/actions/workflows/sanitizers.yml)
[![Coverage Status](https://github.com/ariadne-cps/paving/actions/workflows/coverage.yml/badge.svg)](https://github.com/ariadne-cps/paving/actions/workflows/coverage.yml)
[![codecov](https://codecov.io/gh/ariadne-cps/paving/branch/main/graph/badge.svg)](https://codecov.io/gh/ariadne-cps/paving)

Ariadne Paving is the standalone C++20 discrete paving layer of Ariadne. It provides binary trees, grids, cells, grid-tree pavings, set approximation and subdivision algorithms.

## Layout

- Public headers: `include/paving/`, with umbrella header `include/ariadne-paving.hpp`.
- C++ implementations: `src/`.
- C++ tests: `tests/`.
- Python bindings, tests and tutorial: `python/bindings/`, `python/tests/` and `python/tutorials/`.
- Standalone installed C++ example: `tutorials/tutorial_paving/`.

## Dependencies

Paving depends directly on:

- [ariadne-cps/set](https://github.com/ariadne-cps/set) (Git submodule `ss/set`).
- [ariadne-cps/configuration](https://github.com/ariadne-cps/configuration) (Git submodule `ss/configuration`).

Set provides Solving, Function, Geometry and the lower layers transitively.

## Build

```bash
git clone --recurse-submodules https://github.com/ariadne-cps/paving.git
cd paving
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel --target tests
ctest --test-dir build --output-on-failure
```

For installation and Python bindings:

```bash
cmake -S . -B build-release -DCMAKE_BUILD_TYPE=Release
cmake --build build-release --parallel --target ariadne-paving bindings
cmake --install build-release --prefix /path/to/install
```

## Python bindings

Python 3.11 or newer and a compatible development environment are required for the Python extension. Paving contributes grid and paving operations to `pyariadne` through the Set Python module and its dependencies.

## Tutorials

- C++: `tutorials/tutorial_paving/tutorial_paving.cpp`, built separately against an installed `ariadne-paving`.
- Python: `python/tutorials/tutorial_paving.py`.

## Contribution guidelines

Please contact the developer: Luca Geretti <luca.geretti@univr.it>.

## License

Ariadne Paving is released under the GNU General Public License v3.0 (or later where individual source files specify it).
