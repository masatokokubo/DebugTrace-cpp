cmake_minimum_required(VERSION 4.4.3)

# Enable import std before project() discovers the C++ toolchain.
# UUID for CMake 4.4.3: Help/dev/experimental.rst in the CMake source tree.
set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "f35a9ac6-8463-4d38-8eec-5d6008153e7d")

project(debugtrace_readme_example LANGUAGES CXX)

if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR
   CMAKE_CXX_COMPILER_VERSION VERSION_LESS 21.1.8)
    message(FATAL_ERROR "This example requires clang++ 21.1.8 or newer. Configure with -DCMAKE_CXX_COMPILER=clang++.")
endif()

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
