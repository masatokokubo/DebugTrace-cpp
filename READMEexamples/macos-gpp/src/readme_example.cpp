import std;
import debugtrace;
import point;

void sub() {
    debugtrace::Enter enter(std::source_location::current());
    auto p1 = example::Point<int>(10, 20);
    auto p2 = example::Point<int>(3, 4);
    auto p3 = p1 + p2;
    auto pp = &p3;
    std::vector<example::Point<int>> p_v = {p1, p2, p3};
    debugtrace::print("p1", p1);
    debugtrace::print("p2", p2);
    debugtrace::print("p1 - p2", p1 - p2);
    debugtrace::print("p1 * p2", p1 * p2);
    debugtrace::print("p1 / p2", p1 / p2);
    debugtrace::print("p1 % p2", p1 % p2);
    debugtrace::print("pp", pp);
    debugtrace::print("p_v", p_v);
}

int main() {
    debugtrace::Enter enter(std::source_location::current());
    sub();
    return 0;
}
