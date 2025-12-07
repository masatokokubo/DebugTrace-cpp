## DebugTrace-cpp

[[Japanese]](README_ja.md)

DebugTrace-c	pp is a C++ library for outputting debugging logs.
To use it, simply download DebugTrace-cpp.zip and include the included debugtrace.hpp.
This library contains only header files; there are no files to link (*.lib, *.a, etc.).
Your C++ compiler must support C++14 or later.

### How to use

Use the following macros.

| Macro name (argument)            | Description                                                                                                 |
| :------------------------------- | :---------------------------------------------------------------------------------------------------------- |
| `DEBUGTRACE_VARIABLES`    　　 | Write it only in one of the source files (for example, the file containing the main function).              |
| `DEBUGTRACE_ENTER`             | By describing it at the start position of the function, outputs a log at the start and end of the function. |
| `DEBUGTRACE_MESSAGE(message)`  | Outputs the message.                                                                                        |
| `DEBUGTRACE_PRINT(var)`        | Outputs the name and value of the variable.                                                                 |

### Example of use

```
#include <string>
#include <vector>

template <typename T> class Point {
    private:
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

### Example of execution (on Linux compiled with g++ 14.2.1)
```
2025-12-07 16:01:50+0900 DebugTrace-cpp 2.0.0 compiled with g++ 14.2.1 20250110 (Red Hat 14.2.1-7) (202302L)
2025-12-07 16:01:50+0900 
2025-12-07 16:01:50+0900 Enter int main(int, char**) (readme-example.cpp: 44)
2025-12-07 16:01:50+0900 | Enter void sub() (readme-example.cpp: 30)
2025-12-07 16:01:50+0900 | | p1 = (Point<int>)(1, 2)
2025-12-07 16:01:50+0900 | | p2 = (Point<int>)(3, 4)
2025-12-07 16:01:50+0900 | | p3 = (Point<int>)(4, 6)
2025-12-07 16:01:50+0900 | | pp = (Point<int> const*)&(4, 6)
2025-12-07 16:01:50+0900 | | v = (std::vector<Point<int>, std::allocator<Point<int> > > size:3){
2025-12-07 16:01:50+0900 | |   (Point<int>)(1, 2),
2025-12-07 16:01:50+0900 | |   (Point<int>)(3, 4),
2025-12-07 16:01:50+0900 | |   (Point<int>)(4, 6),
2025-12-07 16:01:50+0900 | | }
2025-12-07 16:01:50+0900 | Leave void sub() (readme-example.cpp)
2025-12-07 16:01:50+0900 Leave int main(int, char**) (readme-example.cpp)
```

### Example of execution (on Linux compiled with Clang 19.1.7)
```
2025-12-07 16:02:26+0900 DebugTrace-cpp 2.0.0 compiled with g++ Clang 19.1.7 (AlmaLinux OS Foundation 19.1.7-2.el10.alma.1) (202302L)
2025-12-07 16:02:26+0900 
2025-12-07 16:02:26+0900 Enter int main(int, char **) (readme-example.cpp: 44)
2025-12-07 16:02:26+0900 | Enter void sub() (readme-example.cpp: 30)
2025-12-07 16:02:26+0900 | | p1 = (Point<int>)(1, 2)
2025-12-07 16:02:26+0900 | | p2 = (Point<int>)(3, 4)
2025-12-07 16:02:26+0900 | | p3 = (Point<int>)(4, 6)
2025-12-07 16:02:26+0900 | | pp = (Point<int> const*)&(4, 6)
2025-12-07 16:02:26+0900 | | v = (std::vector<Point<int>, std::allocator<Point<int> > > size:3){
2025-12-07 16:02:26+0900 | |   (Point<int>)(1, 2),
2025-12-07 16:02:26+0900 | |   (Point<int>)(3, 4),
2025-12-07 16:02:26+0900 | |   (Point<int>)(4, 6),
2025-12-07 16:02:26+0900 | | }
2025-12-07 16:02:26+0900 | Leave void sub() (readme-example.cpp)
2025-12-07 16:02:26+0900 Leave int main(int, char **) (readme-example.cpp)
```

### Example of execution (on Windows compiled with Visual C++ 18.0.0)
```
2025-12-07 15:59:45+0900 DebugTrace-cpp 2.0.0 compiled with Microsoft Visual C++ 195035719 (202302L)
2025-12-07 15:59:45+0900
2025-12-07 15:59:45+0900 Enter int __cdecl main(void) (READMEexample.cpp: 45)
2025-12-07 15:59:45+0900 | Enter void __cdecl sub(void) (READMEexample.cpp: 31)
2025-12-07 15:59:45+0900 | | p1 = (class Point<int>)(1, 2)
2025-12-07 15:59:45+0900 | | p2 = (class Point<int>)(3, 4)
2025-12-07 15:59:45+0900 | | p3 = (class Point<int>)(4, 6)
2025-12-07 15:59:45+0900 | | pp = (class Point<int> const * __ptr64)&(4, 6)
2025-12-07 15:59:45+0900 | | v = (class std::vector<class Point<int>,class std::allocator<class Point<int> > > size:3){
2025-12-07 15:59:45+0900 | |   (class Point<int>)(1, 2),
2025-12-07 15:59:45+0900 | |   (class Point<int>)(3, 4),
2025-12-07 15:59:45+0900 | |   (class Point<int>)(4, 6),
2025-12-07 15:59:45+0900 | | }
2025-12-07 15:59:45+0900 | Leave void __cdecl sub(void) (READMEexample.cpp)
2025-12-07 15:59:45+0900 Leave int __cdecl main(void) (READMEexample.cpp)
```
