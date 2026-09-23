# Building on Linux with g++

[Japanese](README_ja.md)

Feature requirements: CMake 4.0, g++ 15.1, and Ninja 1.11 or newer.
The minimum-version combination has not been tested.

Tested with CMake 4.4.3, g++ 15.2.1, and Ninja 1.11.1.
The libstdc++ standard library modules matching your g++ installation are required.

Run the following commands from this directory:

```sh
cmake -S . -B build -G Ninja -DCMAKE_CXX_COMPILER=g++ -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel
./build/debugtrace-example
```

For a release build, replace `-DCMAKE_BUILD_TYPE=Debug` with
`-DCMAKE_BUILD_TYPE=Release` in the configure command.
To clean the build artifacts, run `cmake --build build --target clean`.

The build compiles `src/readme_example.cpp`, `src/point.cppm`, and
`../../src/debugtrace.cppm` as C++23. CMake also builds the standard library's
`std` module and determines the compilation order from module dependencies.

CMake support for `import std;` is experimental, so a warning appears during
configuration. `CMakeLists.txt` selects the activation UUID for CMake 4.0 through 4.4.
For later CMake versions, check the experimental feature documentation for the version
you are using; see the
[CMake 4.4.3 documentation](https://github.com/Kitware/CMake/blob/v4.4.3/Help/dev/experimental.rst#c-import-std-support)
for the current setting.
