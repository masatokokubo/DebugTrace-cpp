## DebugTrace-cpp

[[English]](README.md)

DebugTrace-cpp は、デバッグ用のログを出力するためのC++ライブラリです。  
DebugTrace-cpp.zip をダウンロードし、この中に含まれている debugtrace.hpp をインクルードするだけで使用できます。  
ヘッダーファイルだけのライブラリで、リンクするファイル (*.lib, *.a など) はありません。  
C++コンパイラは、C++14以降に対応している必要があります。

#### 使用方法

以下のマクロを使用します。

|マクロ名(引数)|説明
|:-----------|:---
|`DEBUGTRACE_VARIABLES`       |<b>C++17以降では不要</b><br>いずれかのソースファイル (例えば main 関数を含むファイル) にだけ記述する
|`DEBUGTRACE_ENTER`           |関数の開始位置に記述する事で、その関数の開始および終了時にログを出力する
|`DEBUGTRACE_MESSAGE(message)`|メッセージを出力する
|`DEBUGTRACE_PRINT(var)`      |変数の名前と値を出力する

#### ログの出力先の指定
`debugtrace::output_target` に設定する事でログの出力先を変更できます。

|指定値|出力先
|:----|:----
|`"std::cout"`   |標準出力<br>例: <code>`debugtrace::output_target = "std::cout";`
|`"std::cerr"`   |標準エラー出力 (デフォルト)<br>例: <code>`debugtrace::output_target = "std::cerr";`
|`"<ファイルパス>"`|指定のファイル<br>先頭に + を付加すると追加出力<br>例:<br>`debugtrace::output_target = "/tmp/debugtrace.log"`<br>`debugtrace::output_target = "+/tmp/debugtrace.log"`

#### 使用例
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

#### 実行例 (Linux / g++ 14.3.1)
```
026-05-06 11:25:37+0900 DebugTrace-cpp 2.1.0 compiled with g++ 14.3.1 20250617 (Red Hat 14.3.1-2) (202302L)
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

#### 実行例 (Linux / Clang 20.1.8)
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

#### 実行例 (Windows / Visual C++ 19.50)
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

#### 変更履歴

#### 2.1.0 - 2026/6/5 
* ログの出力先の指定に対応。
