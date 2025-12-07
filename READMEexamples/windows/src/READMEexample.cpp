#include "stdafx.h"
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

template <typename T> auto operator +(Point<T> const& p1, Point<T> const& p2) noexcept {
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

int main() {
    DEBUGTRACE_ENTER
    sub();
    return 0;
}
