module;

export module point;
import std;

export namespace example {
    template <typename T>
    class Point {
        T _x = 0;
        T _y = 0;
    public:
        Point(T const& x, T const& y) noexcept : _x(x), _y(y) {}
        auto x() const noexcept {return _x;}
        auto y() const noexcept {return _y;}
    };

    template <typename T>
    auto operator +(Point<T> const& p1, Point<T> const& p2) noexcept {
        return Point<T>(p1.x() + p2.x(), p1.y() + p2.y());
    }

    template <typename T>
    auto operator -(Point<T> const& p1, Point<T> const& p2) noexcept {
        return Point<T>(p1.x() - p2.x(), p1.y() - p2.y());
    }

    template <typename T>
    auto operator *(Point<T> const& p1, Point<T> const& p2) noexcept {
        return Point<T>(p1.x() * p2.x(), p1.y() * p2.y());
    }

    template <typename T>
    auto operator /(Point<T> const& p1, Point<T> const& p2) noexcept {
        return Point<T>(p1.x() / p2.x(), p1.y() / p2.y());
    }

    template <typename T>
    auto operator %(Point<T> const& p1, Point<T> const& p2) noexcept {
        return Point<T>(p1.x() % p2.x(), p1.y() % p2.y());
    }

    template <typename T>
    std::string to_string(Point<T> const& p) {
        return "(" + std::to_string(p.x()) + ", " + std::to_string(p.y()) + ")";
    }
}
