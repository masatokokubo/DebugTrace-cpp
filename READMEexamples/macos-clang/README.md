# Building on macOS with Clang

[Japanese](README_ja.md)

Tested on macOS 27.0 (Apple Silicon) with CMake 4.4.3, Homebrew LLVM
23.1.1 (libc++), and Ninja 1.13.2.
This example requires CMake 4.4.3 or newer and LLVM Clang 21.1.8 or newer,
including `clang-scan-deps` and the C++23 `std` module.

Install the build tools:

```sh
brew install cmake ninja llvm
```

Run the following commands from this directory:

```sh
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel
./build/debugtrace-example
```

CMake automatically selects Homebrew LLVM from `/opt/homebrew/opt/llvm`
(Apple Silicon) or `/usr/local/opt/llvm` (Intel). Apple Clang in
`/usr/bin/clang++` is not supported by this example.
If `CXX` is set, or you use a different LLVM installation, select the compiler
explicitly in a fresh build directory:

```sh
cmake -S . -B build/llvm -G Ninja \
  -DCMAKE_CXX_COMPILER="$(brew --prefix llvm)/bin/clang++" \
  -DCMAKE_BUILD_TYPE=Debug
cmake --build build/llvm --parallel
./build/llvm/debugtrace-example
```

For a release build, use `-DCMAKE_BUILD_TYPE=Release`.
Use `cmake --build build --target clean` to remove build artifacts.

The build compiles `src/readme_example.cpp`, `src/point.cppm`, and
`../../src/debugtrace.cppm` as C++23. CMake builds the standard library's
`std` module and determines compilation order from module dependencies.
The configuration locates `lib/c++/libc++.modules.json` relative to the LLVM
compiler and uses that installation's libc++ headers and runtime.
An executable rpath allows the example to run without `DYLD_LIBRARY_PATH`.

CMake's [`import std;` support](https://cmake.org/cmake/help/v4.4/prop_tgt/CXX_MODULE_STD.html)
is experimental. Configuration emits a warning, and Clang may emit
`-Wreserved-module-identifier` warnings for the standard library modules.
The enabling UUID in `CMakeLists.txt` is for CMake 4.4.3; when upgrading,
check the [experimental feature documentation](https://github.com/Kitware/CMake/blob/v4.4.3/Help/dev/experimental.rst#c-import-std-support)
for your CMake version.
