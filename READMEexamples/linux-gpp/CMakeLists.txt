cmake_minimum_required(VERSION 4.0)

# Enable import std before project() discovers the C++ toolchain.
# UUIDs from Help/dev/experimental.rst in each CMake release.
if(CMAKE_VERSION VERSION_LESS 4.1)
    set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "a9e1cf81-9932-4810-974b-6eccaf14e457")
elseif(CMAKE_VERSION VERSION_LESS 4.3)
    set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "d0edc3af-4c50-42ea-a356-e2862fe7a444")
elseif(CMAKE_VERSION VERSION_LESS 4.4)
    set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "451f2fe2-a8a2-47c3-bc32-94786d8fc91b")
else()
    set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "f35a9ac6-8463-4d38-8eec-5d6008153e7d")
endif()

project(debugtrace_readme_example LANGUAGES CXX)

if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR
   CMAKE_CXX_COMPILER_VERSION VERSION_LESS 15.1)
    message(FATAL_ERROR "This example requires g++ 15.1 or newer.")
endif()

if(NOT 23 IN_LIST CMAKE_CXX_COMPILER_IMPORT_STD)
    message(FATAL_ERROR "The C++ toolchain must provide the libstdc++ C++23 std module.")
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
