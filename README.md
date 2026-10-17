# MiltLibs

[![CI](https://github.com/miltonmcdonald1986/MiltLibs/actions/workflows/ci.yml/badge.svg?branch=main&event=push)](https://github.com/miltonmcdonald1986/MiltLibs/actions/workflows/ci.yml?query=branch%3Amain)

Milton's C++23 libraries.

## Building

Requires CMake 3.28+, Ninja, and a C++23 compiler. On Windows, use a Visual Studio Developer prompt.

```sh
cmake --preset msvc-debug
cmake --build --preset msvc-debug
ctest --preset msvc-debug
```

Presets: `msvc-debug`, `msvc-release`, `clang-debug`, `clang-release`, `gcc-debug`, `gcc-release`.
