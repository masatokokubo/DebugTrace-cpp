## DebugTrace-cpp

[[Japanese]](README_ja.md)

DebugTrace-c	pp is a C++ library for outputting debugging logs.
To use it, simply download DebugTrace-cpp.zip and include the included debugtrace.hpp.
This library contains only header files; there are no files to link (*.lib, *.a, etc.).
Your C++ compiler must support C++14 or later.

### How to use

Use the following macros.

|Macro name (argument)        |Description
|:----------------------------|:----------
|`DEBUGTRACE_VARIABLES`       |<b>This is not necessary in C++17 and later.</b><br>Write it only in one of the source files (for example, the file containing the main function).
|`DEBUGTRACE_ENTER`           |By describing it at the start position of the function, outputs a log at the start and end of the function.
|`DEBUGTRACE_MESSAGE(message)`|Outputs the message.
|`DEBUGTRACE_PRINT(var)`      |Outputs the name and value of the variable.

### Specifying the Log Output Destination
You can change the log output destination by setting it in `debugtrace::output_target`.

|Specified Value|Output Destination
|:--------------|:-----------------
|`"std::cout"`  |Standard Output<br>Example: `debugtrace::output_target = "std::cerr"`
|`"std::cerr"`  |Standard Error Output (Default)<br>Example: `debugtrace::output_target = "std::cerr"`
|`"<File Path>"`|Specified File<br>Adding a + at the beginning will append to the output.<br>Examples:<br>`debugtrace::output_target = "/tmp/debugtrace.log"`<br>`debugtrace::output_target = "+/tmp/debugtrace.log"`

### Example of use

```
#include <string>
#include <vector>

template <typename T> class Point {
    T _x = 0;
    T _y = 0;
public:
    Point(T const& x, T const& y) noexcept : _x(x), _y(y) {}
    auto x() const noexcept {return _x;}
    auto y() const noexcept {return _y;}
};

template <typename T> auto operator +(const Point<T>& p1, const Point<T>& p2) noexcept {
    return Point<T>(p1.x() + p2.x(), p1.y() + p2.y());
}

namespace std {
    template <typename T>
    string to_string(const Point<T>& p) {
        return '(' + to_string(p.x()) + ", " + to_string(p.y()) + ')';
    }
}

#include "debugtrace.hpp"

DEBUGTRACE_VARIABLES // Describe in only one source.

void sub() {
    DEBUGTRACE_ENTER
    Point<int> p1 = Point<int>(1, 2);
    Point<int> p2 = Point<int>(3, 4);
    Point<int> p3 = p1 + p2;
    Point<int> const* pp = &p3;
    std::vector<Point<int>> v = {p1, p2, p3};
    DEBUGTRACE_PRINT(p1)
    DEBUGTRACE_PRINT(p2)
    DEBUGTRACE_PRINT(p3)
    DEBUGTRACE_PRINT(pp)
    DEBUGTRACE_PRINT(v)
}

int main(int argc, char** argv) {
    DEBUGTRACE_ENTER
    sub();
    return 0;
}
```

### Example of execution (on Linux compiled with g++ 14.3.1)
```
2026-05-06 11:25:37+0900 DebugTrace-cpp 2.1.0 compiled with g++ 14.3.1 20250617 (Red Hat 14.3.1-2) (202302L)
2026-05-06 11:25:37+0900 Output: std::cerr
2026-05-06 11:25:37+0900 
2026-05-06 11:25:37+0900 Enter int main(int, char**) (readme-example.cpp: 43)
2026-05-06 11:25:37+0900 | Enter void sub() (readme-example.cpp: 29)
2026-05-06 11:25:37+0900 | | p1 = (Point<int>)(1, 2)
2026-05-06 11:25:37+0900 | | p2 = (Point<int>)(3, 4)
2026-05-06 11:25:37+0900 | | p3 = (Point<int>)(4, 6)
2026-05-06 11:25:37+0900 | | pp = (Point<int> const*)&(4, 6)
2026-05-06 11:25:37+0900 | | v = (std::vector<Point<int>, std::allocator<Point<int> > > size:3){
2026-05-06 11:25:37+0900 | |   (Point<int>)(1, 2),
2026-05-06 11:25:37+0900 | |   (Point<int>)(3, 4),
2026-05-06 11:25:37+0900 | |   (Point<int>)(4, 6),
2026-05-06 11:25:37+0900 | | }
2026-05-06 11:25:37+0900 | Leave void sub() (readme-example.cpp)
2026-05-06 11:25:37+0900 Leave int main(int, char**) (readme-example.cpp)
```

### Example of execution (on Linux compiled with Clang 20.1.8)
```
2026-05-06 11:22:23+0900 DebugTrace-cpp 2.1.0 compiled with g++ Clang 20.1.8 (AlmaLinux OS Foundation 20.1.8-1.el10.alma.1) (202302L)
2026-05-06 11:22:23+0900 Output: std::cerr
2026-05-06 11:22:23+0900 
2026-05-06 11:22:23+0900 Enter int main(int, char **) (readme-example.cpp: 43)
2026-05-06 11:22:23+0900 | Enter void sub() (readme-example.cpp: 29)
2026-05-06 11:22:23+0900 | | p1 = (Point<int>)(1, 2)
2026-05-06 11:22:23+0900 | | p2 = (Point<int>)(3, 4)
2026-05-06 11:22:23+0900 | | p3 = (Point<int>)(4, 6)
2026-05-06 11:22:23+0900 | | pp = (Point<int> const*)&(4, 6)
2026-05-06 11:22:23+0900 | | v = (std::vector<Point<int>, std::allocator<Point<int> > > size:3){
2026-05-06 11:22:23+0900 | |   (Point<int>)(1, 2),
2026-05-06 11:22:23+0900 | |   (Point<int>)(3, 4),
2026-05-06 11:22:23+0900 | |   (Point<int>)(4, 6),
2026-05-06 11:22:23+0900 | | }
2026-05-06 11:22:23+0900 | Leave void sub() (readme-example.cpp)
2026-05-06 11:22:23+0900 Leave int main(int, char **) (readme-example.cpp)
```

### Example of execution (on Windows compiled with Visual C++ 19.50)
```
2026-05-06 09:52:52+0900 DebugTrace-cpp 2.1.0 compiled with Microsoft Visual C++ 195035730 (202302L)
2026-05-06 09:52:52+0900 Output: std::cerr
2026-05-06 09:52:52+0900
2026-05-06 09:52:52+0900 Enter int __cdecl main(void) (READMEexample.cpp: 44)
2026-05-06 09:52:52+0900 | Enter void __cdecl sub(void) (READMEexample.cpp: 30)
2026-05-06 09:52:52+0900 | | p1 = (class Point<int>)(1, 2)
2026-05-06 09:52:52+0900 | | p2 = (class Point<int>)(3, 4)
2026-05-06 09:52:52+0900 | | p3 = (class Point<int>)(4, 6)
2026-05-06 09:52:52+0900 | | pp = (class Point<int> const * __ptr64)&(4, 6)
2026-05-06 09:52:52+0900 | | v = (class std::vector<class Point<int>,class std::allocator<class Point<int> > > size:3){
2026-05-06 09:52:52+0900 | |   (class Point<int>)(1, 2),
2026-05-06 09:52:52+0900 | |   (class Point<int>)(3, 4),
2026-05-06 09:52:52+0900 | |   (class Point<int>)(4, 6),
2026-05-06 09:52:52+0900 | | }
2026-05-06 09:52:52+0900 | Leave void __cdecl sub(void) (READMEexample.cpp)
2026-05-06 09:52:52+0900 Leave int __cdecl main(void) (READMEexample.cpp)
```

### CHANGELOG

#### 2.1.0 - May 6, 2026
* Supports specifying the log output destination.

