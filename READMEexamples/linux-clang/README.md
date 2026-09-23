# Building on Linux with Clang

[Japanese](README_ja.md)

Tested with CMake 4.4.3, Clang 21.1.8, Ninja 1.11.1,
and libstdc++ from GCC Toolset 15.
You need `clang-scan-deps` matching your Clang version and a standard library
that provides the C++23 `std` module.

Run the following commands from this directory:

```sh
cmake -S . -B build -G Ninja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel
./build/debugtrace-example
```

For a release build, replace `-DCMAKE_BUILD_TYPE=Debug` with
`-DCMAKE_BUILD_TYPE=Release` in the configuration command.
Use `cmake --build build --target clean` to remove build artifacts.

The build compiles `src/readme_example.cpp`, `src/point.cppm`, and
`../../src/debugtrace.cppm` as C++23. CMake also builds the standard library's
`std` module and determines the compilation order from module dependencies.

The build uses Clang's default standard library configuration. In the tested
environment, `clang++ -print-file-name=libstdc++.modules.json` resolves to the
module metadata file provided by GCC Toolset 15.

CMake 4.4.3's [`import std;` support](https://cmake.org/cmake/help/v4.4/prop_tgt/CXX_MODULE_STD.html)
is experimental, so CMake displays a warning during configuration.
In this environment, Clang also emits `-Wreserved-module-identifier` warnings
when building the standard library, but the build and execution succeed.
The UUID that enables this feature in `CMakeLists.txt` is specific to CMake 4.4.3.
When upgrading CMake, check the
[experimental feature documentation for the version you use](https://github.com/Kitware/CMake/blob/v4.4.3/Help/dev/experimental.rst#c-import-std-support).
