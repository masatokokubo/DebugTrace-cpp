# Building on macOS with GNU g++

[Japanese](README_ja.md)

Feature requirements: CMake 4.0, g++ 15.1, and Ninja 1.11 or newer.
The minimum-version combination has not been tested.

Build and execution verified on macOS arm64 with CMake 4.4.3,
Homebrew g++ 16.2.0, and Ninja 1.13.2.
The libstdc++ standard library modules matching your g++ installation are required.

Install Xcode Command Line Tools (`xcode-select --install`) if needed, then
install the build tools with Homebrew:

```sh
brew install cmake gcc ninja
```

macOS `/usr/bin/g++` is Apple Clang. This project automatically searches for
`g++-16` and `g++-15`, including the Homebrew locations on Apple Silicon and
Intel Macs. To select another GNU g++ installation, pass
`-DCMAKE_CXX_COMPILER=/path/to/g++` when configuring. An explicit CMake compiler,
`CXX` environment variable, or toolchain file takes precedence.

Run the following commands from this directory:

```sh
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel
./build/debugtrace-example
```

If `build` was configured with another compiler, use a new build directory
(such as `build-gcc`) consistently in all three commands. Ninja is required
for CMake to track C++ module dependencies.

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
