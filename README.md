# orbifold

A lightweight C++ implementation that mirrors the ideas in the paper behind the [`euclidean_orbifolds`](https://github.com/noamaig/euclidean_orbifolds) project. The library focuses on parsing Conway orbifold symbols, computing a simple Euler characteristic estimate, and mapping wallpaper orbifold symbols to their wallpaper group names. Everything is wired for CMake/CLion so you can open the folder and build immediately.

## Building

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build
```

The `orbifold_example` target prints the parsed properties for several common wallpaper orbifolds.

## Usage example

```cpp
#include "orbifold/OrbifoldSymbol.h"

using orbifold::OrbifoldSymbol;

int main() {
    OrbifoldSymbol p6m("*632");
    std::cout << p6m.wallpaperGroup() << "\n"; // p6m
    std::cout << p6m.eulerCharacteristic() << "\n";
}
```

The parser understands digits (orders of cone points or corner reflectors), `*` for mirrored boundaries, `x` for crosscaps, and `o` for handles. A lookup table covers the standard wallpaper orbifold symbols (`*632`, `4*2`, `*x`, etc.) and raises an exception for unknown entries.

## Notes

The code intentionally stays dependency‑free: only the C++20 standard library is required. The Euler characteristic uses a straightforward approximation suitable for quick exploration of wallpaper orbifolds.

