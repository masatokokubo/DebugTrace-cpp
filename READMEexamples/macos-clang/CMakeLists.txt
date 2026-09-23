cmake_minimum_required(VERSION 4.4.3)

if(NOT CMAKE_GENERATOR MATCHES "^Ninja")
    message(FATAL_ERROR "C++23 modules require Ninja. Configure with -G Ninja.")
endif()

# Prefer Homebrew LLVM over /usr/bin/clang++ (Apple Clang).
if(NOT DEFINED CMAKE_CXX_COMPILER AND "$ENV{CXX}" STREQUAL "" AND
   NOT DEFINED CMAKE_TOOLCHAIN_FILE AND "$ENV{CMAKE_TOOLCHAIN_FILE}" STREQUAL "")
    find_program(DEBUGTRACE_CLANGXX NAMES clang++
        PATHS /opt/homebrew/opt/llvm/bin /usr/local/opt/llvm/bin
        NO_DEFAULT_PATH)
    if(NOT DEBUGTRACE_CLANGXX)
        message(FATAL_ERROR
            "Install Homebrew LLVM with 'brew install llvm', or specify "
            "-DCMAKE_CXX_COMPILER=/path/to/llvm/bin/clang++.")
    endif()
    set(CMAKE_CXX_COMPILER "${DEBUGTRACE_CLANGXX}" CACHE FILEPATH "C++ compiler")
endif()

# Enable import std before project() discovers the C++ toolchain.
# UUID for CMake 4.4.3: Help/dev/experimental.rst in the CMake source tree.
set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "f35a9ac6-8463-4d38-8eec-5d6008153e7d")

project(debugtrace_readme_example LANGUAGES CXX)

if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR
   CMAKE_CXX_COMPILER_VERSION VERSION_LESS 21.1.8)
    message(FATAL_ERROR
        "This example requires LLVM clang++ 21.1.8 or newer. "
        "Configure a fresh build directory with "
        "-DCMAKE_CXX_COMPILER=$(brew --prefix llvm)/bin/clang++. "
        "Apple Clang is not supported by this example.")
endif()

# Homebrew's Clang does not locate libc++.modules.json automatically on macOS.
# Use the headers, module sources and runtime from the same LLVM installation.
get_filename_component(_llvm_bin "${CMAKE_CXX_COMPILER}" DIRECTORY)
get_filename_component(_llvm_prefix "${_llvm_bin}/.." ABSOLUTE)
if(NOT CMAKE_CXX_STDLIB_MODULES_JSON OR
   CMAKE_CXX_STDLIB_MODULES_JSON STREQUAL "libc++.modules.json")
    set(CMAKE_CXX_STDLIB_MODULES_JSON
        "${_llvm_prefix}/lib/c++/libc++.modules.json")
endif()
if(NOT EXISTS "${CMAKE_CXX_STDLIB_MODULES_JSON}" OR
   NOT EXISTS "${_llvm_prefix}/include/c++/v1" OR
   NOT EXISTS "${_llvm_prefix}/lib/c++/libc++.dylib")
    message(FATAL_ERROR "LLVM must include libc++ headers, runtime and libc++.modules.json.")
endif()

# Apply these flags to CMake's generated std module target as well.
string(APPEND CMAKE_CXX_FLAGS " -nostdinc++ -isystem \"${_llvm_prefix}/include/c++/v1\"")
string(APPEND CMAKE_EXE_LINKER_FLAGS " -L\"${_llvm_prefix}/lib/c++\" -Wl,-rpath,\"${_llvm_prefix}/lib/c++\"")

if(NOT 23 IN_LIST CMAKE_CXX_COMPILER_IMPORT_STD)
    message(FATAL_ERROR "The C++ toolchain must provide a C++23 std module (libstdc++ or libc++).")
endif()

add_executable(debugtrace-example src/readme_example.cpp)
set_target_properties(debugtrace-example PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    CXX_MODULE_STD ON
)
target_sources(debugtrace-example PRIVATE
    FILE_SET CXX_MODULES
    BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/../.."
    FILES
        ../../src/debugtrace.cppm
        src/point.cppm
)
