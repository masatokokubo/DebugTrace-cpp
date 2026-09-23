## DebugTrace-cpp

[English](README.md)

DebugTrace-cpp は、デバッグ用のログを出力するためのC++モジュールです。  
DebugTrace-cpp.zip をダウンロードし、この中に含まれている debugtrace.cppm をインポートする事で使用できます。  
バージョン 3.0.0よりC++23以降の対応になりました。具体的に使用するC++コンパイラに以下の機能が必要です。  
* P0734R0: Contents (C++20)
* P1103R3: Merging Modules (C++20)
* P2465R3: Standard Library Modules std and std.compat (C++23)

|コンパイラ     |対応可能なバージョン
|:------------|:-----------
|GCC          |15.1 以降
|Clang, libc++|19 以降
|Visual C++   |VS2022 17.6 以降

#### 使用方法

1. ログを表示したい関数の先頭で `debugtrace::Enter` クラスの変数を定義します。  
  このクラスのコンストラクタとデストラクタが関数の開始と終了のログを出力します。
1. 内容を出力したい変数があれば、`debugtrace::print` 関数を呼び出します。

#### 使用例
```C++
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
```

#### 実行例 - Linux / g++ 15.2.1
```log
2026-09-21 11:13:45+0900 DebugTrace-cpp 3.0.0 compiled with g++ 15.2.1 20260123 (Red Hat 15.2.1-7) (202302L)
2026-09-21 11:13:45+0900 Output: std::cerr
2026-09-21 11:13:45+0900 
2026-09-21 11:13:45+0900 Enter int main() (readme_example.cpp: 23)
2026-09-21 11:13:45+0900 | Enter void sub() (readme_example.cpp: 6)
2026-09-21 11:13:45+0900 | | p1 = (example::Point@point<int>)(10, 20) (readme_example.cpp: 12)
2026-09-21 11:13:45+0900 | | p2 = (example::Point@point<int>)(3, 4) (readme_example.cpp: 13)
2026-09-21 11:13:45+0900 | | p1 - p2 = (example::Point@point<int>)(7, 16) (readme_example.cpp: 14)
2026-09-21 11:13:45+0900 | | p1 * p2 = (example::Point@point<int>)(30, 80) (readme_example.cpp: 15)
2026-09-21 11:13:45+0900 | | p1 / p2 = (example::Point@point<int>)(3, 5) (readme_example.cpp: 16)
2026-09-21 11:13:45+0900 | | p1 % p2 = (example::Point@point<int>)(1, 0) (readme_example.cpp: 17)
2026-09-21 11:13:45+0900 | | pp = (example::Point@point<int>*)&(13, 24) (readme_example.cpp: 18)
2026-09-21 11:13:45+0900 | | p_v = (std::vector<example::Point@point<int>, std::allocator<example::Point@point<int> > > size:3){ (readme_example.cpp: 19)
2026-09-21 11:13:45+0900 | |   (example::Point@point<int>)(10, 20), (readme_example.cpp: 19)
2026-09-21 11:13:45+0900 | |   (example::Point@point<int>)(3, 4), (readme_example.cpp: 19)
2026-09-21 11:13:45+0900 | |   (example::Point@point<int>)(13, 24), (readme_example.cpp: 19)
2026-09-21 11:13:45+0900 | | } (readme_example.cpp: 19)
2026-09-21 11:13:45+0900 | Leave void sub() (readme_example.cpp)
2026-09-21 11:13:45+0900 Leave int main() (readme_example.cpp)
```

#### 実行例 - Linux / Clang 20.1.8
```
2026-09-21 11:13:59+0900 DebugTrace-cpp 3.0.0 compiled with g++ Clang 21.1.8 (AlmaLinux OS Foundation 21.1.8-1.el10.alma.1) (202302L)
2026-09-21 11:13:59+0900 Output: std::cerr
2026-09-21 11:13:59+0900 
2026-09-21 11:13:59+0900 Enter int main() (readme_example.cpp: 23)
2026-09-21 11:13:59+0900 | Enter void sub() (readme_example.cpp: 6)
2026-09-21 11:13:59+0900 | | p1 = (example::Point@point<int>)(10, 20) (readme_example.cpp: 12)
2026-09-21 11:13:59+0900 | | p2 = (example::Point@point<int>)(3, 4) (readme_example.cpp: 13)
2026-09-21 11:13:59+0900 | | p1 - p2 = (example::Point@point<int>)(7, 16) (readme_example.cpp: 14)
2026-09-21 11:13:59+0900 | | p1 * p2 = (example::Point@point<int>)(30, 80) (readme_example.cpp: 15)
2026-09-21 11:13:59+0900 | | p1 / p2 = (example::Point@point<int>)(3, 5) (readme_example.cpp: 16)
2026-09-21 11:13:59+0900 | | p1 % p2 = (example::Point@point<int>)(1, 0) (readme_example.cpp: 17)
2026-09-21 11:13:59+0900 | | pp = (example::Point@point<int>*)&(13, 24) (readme_example.cpp: 18)
2026-09-21 11:13:59+0900 | | p_v = (std::vector<example::Point@point<int>, std::allocator<example::Point@point<int> > > size:3){ (readme_example.cpp: 19)
2026-09-21 11:13:59+0900 | |   (example::Point@point<int>)(10, 20), (readme_example.cpp: 19)
2026-09-21 11:13:59+0900 | |   (example::Point@point<int>)(3, 4), (readme_example.cpp: 19)
2026-09-21 11:13:59+0900 | |   (example::Point@point<int>)(13, 24), (readme_example.cpp: 19)
2026-09-21 11:13:59+0900 | | } (readme_example.cpp: 19)
2026-09-21 11:13:59+0900 | Leave void sub() (readme_example.cpp)
2026-09-21 11:13:59+0900 Leave int main() (readme_example.cpp)
```

#### 実行例 - Windows / Visual C++ 19.51 (Microsoft Visual Studio 2026)
```
2026-09-21 12:18:14+0900 DebugTrace-cpp 3.0.0 compiled with Microsoft Visual C++ 195136257 (202302L)
2026-09-21 12:18:14+0900 Output: std::cerr
2026-09-21 12:18:14+0900
2026-09-21 12:18:14+0900 Enter int __cdecl main(void) (readme_example.cpp: 23)
2026-09-21 12:18:14+0900 | Enter void __cdecl sub(void) (readme_example.cpp: 6)
2026-09-21 12:18:14+0900 | | p1 = (class example::Point<int>[point])(10, 20) (readme_example.cpp: 12)
2026-09-21 12:18:14+0900 | | p2 = (class example::Point<int>[point])(3, 4) (readme_example.cpp: 13)
2026-09-21 12:18:14+0900 | | p1 - p2 = (class example::Point<int>[point])(7, 16) (readme_example.cpp: 14)
2026-09-21 12:18:14+0900 | | p1 * p2 = (class example::Point<int>[point])(30, 80) (readme_example.cpp: 15)
2026-09-21 12:18:14+0900 | | p1 / p2 = (class example::Point<int>[point])(3, 5) (readme_example.cpp: 16)
2026-09-21 12:18:14+0900 | | p1 % p2 = (class example::Point<int>[point])(1, 0) (readme_example.cpp: 17)
2026-09-21 12:18:14+0900 | | pp = (class example::Point<int>[point] * __ptr64)&(13, 24) (readme_example.cpp: 18)
2026-09-21 12:18:14+0900 | | p_v = (class std::vector<class example::Point<int>[point],class std::allocator<class example::Point<int>[point]> > size:3){ (readme_example.cpp: 19)
2026-09-21 12:18:14+0900 | |   (class example::Point<int>[point])(10, 20), (readme_example.cpp: 19)
2026-09-21 12:18:14+0900 | |   (class example::Point<int>[point])(3, 4), (readme_example.cpp: 19)
2026-09-21 12:18:14+0900 | |   (class example::Point<int>[point])(13, 24), (readme_example.cpp: 19)
2026-09-21 12:18:14+0900 | | } (readme_example.cpp: 19)
2026-09-21 12:18:14+0900 | Leave void __cdecl sub(void) (readme_example.cpp)
2026-09-21 12:18:14+0900 Leave int __cdecl main(void) (readme_example.cpp)```
```

#### 実行例 - macOS / g++ 16.2.0
```
2026-09-23 12:05:27+0900 DebugTrace-cpp 3.0.0 compiled with g++ 16.2.0 (202302L)
2026-09-23 12:05:27+0900 Output: std::cerr
2026-09-23 12:05:27+0900 
2026-09-23 12:05:27+0900 Enter int main() (readme_example.cpp: 23)
2026-09-23 12:05:27+0900 | Enter void sub() (readme_example.cpp: 6)
2026-09-23 12:05:27+0900 | | p1 = (example::Point@point<int>)(10, 20) (readme_example.cpp: 12)
2026-09-23 12:05:27+0900 | | p2 = (example::Point@point<int>)(3, 4) (readme_example.cpp: 13)
2026-09-23 12:05:27+0900 | | p1 - p2 = (example::Point@point<int>)(7, 16) (readme_example.cpp: 14)
2026-09-23 12:05:27+0900 | | p1 * p2 = (example::Point@point<int>)(30, 80) (readme_example.cpp: 15)
2026-09-23 12:05:27+0900 | | p1 / p2 = (example::Point@point<int>)(3, 5) (readme_example.cpp: 16)
2026-09-23 12:05:27+0900 | | p1 % p2 = (example::Point@point<int>)(1, 0) (readme_example.cpp: 17)
2026-09-23 12:05:27+0900 | | pp = (example::Point@point<int>*)&(13, 24) (readme_example.cpp: 18)
2026-09-23 12:05:27+0900 | | p_v = (std::vector<example::Point@point<int>, std::allocator<example::Point@point<int> > > size:3){ (readme_example.cpp: 19)
2026-09-23 12:05:27+0900 | |   (example::Point@point<int>)(10, 20), (readme_example.cpp: 19)
2026-09-23 12:05:27+0900 | |   (example::Point@point<int>)(3, 4), (readme_example.cpp: 19)
2026-09-23 12:05:27+0900 | |   (example::Point@point<int>)(13, 24), (readme_example.cpp: 19)
2026-09-23 12:05:27+0900 | | } (readme_example.cpp: 19)
2026-09-23 12:05:27+0900 | Leave void sub() (readme_example.cpp)
2026-09-23 12:05:27+0900 Leave int main() (readme_example.cpp)
```

#### 実行例 - macOS / Clang 23.1.1
```
masato@devel-mac-arm macos-clang % ./build/debugtrace-example
2026-09-23 12:10:39+0900 DebugTrace-cpp 3.0.0 compiled with g++ Homebrew Clang 23.1.1 (202302L)
2026-09-23 12:10:39+0900 Output: std::cerr
2026-09-23 12:10:39+0900 
2026-09-23 12:10:39+0900 Enter int main() (readme_example.cpp: 23)
2026-09-23 12:10:39+0900 | Enter void sub() (readme_example.cpp: 6)
2026-09-23 12:10:39+0900 | | p1 = (example::Point@point<int>)(10, 20) (readme_example.cpp: 12)
2026-09-23 12:10:39+0900 | | p2 = (example::Point@point<int>)(3, 4) (readme_example.cpp: 13)
2026-09-23 12:10:39+0900 | | p1 - p2 = (example::Point@point<int>)(7, 16) (readme_example.cpp: 14)
2026-09-23 12:10:39+0900 | | p1 * p2 = (example::Point@point<int>)(30, 80) (readme_example.cpp: 15)
2026-09-23 12:10:39+0900 | | p1 / p2 = (example::Point@point<int>)(3, 5) (readme_example.cpp: 16)
2026-09-23 12:10:39+0900 | | p1 % p2 = (example::Point@point<int>)(1, 0) (readme_example.cpp: 17)
2026-09-23 12:10:39+0900 | | pp = (example::Point@point<int>*)&(13, 24) (readme_example.cpp: 18)
2026-09-23 12:10:39+0900 | | p_v = (std::__1::vector<example::Point@point<int>, std::__1::allocator<example::Point@point<int>>> size:3){ (readme_example.cpp: 19)
2026-09-23 12:10:39+0900 | |   (example::Point@point<int>)(10, 20), (readme_example.cpp: 19)
2026-09-23 12:10:39+0900 | |   (example::Point@point<int>)(3, 4), (readme_example.cpp: 19)
2026-09-23 12:10:39+0900 | |   (example::Point@point<int>)(13, 24), (readme_example.cpp: 19)
2026-09-23 12:10:39+0900 | | } (readme_example.cpp: 19)
2026-09-23 12:10:39+0900 | Leave void sub() (readme_example.cpp)
2026-09-23 12:10:39+0900 Leave int main() (readme_example.cpp)
```

#### ログの出力先の指定
`debugtrace::output_target` に設定する事でログの出力先を変更できます。

|指定値|出力先
|:----|:----
|`"std::cout"`   |標準出力<br>例: <code>`debugtrace::output_target = "std::cout";`
|`"std::cerr"`   |標準エラー出力 (デフォルト)<br>例: <code>`debugtrace::output_target = "std::cerr";`
|`"<ファイルパス>"`|指定のファイル<br>先頭に + を付加すると追加出力<br>例:<br>`debugtrace::output_target = "/tmp/debugtrace.log"`<br>`debugtrace::output_target = "+/tmp/debugtrace.log"`

#### 変更履歴
[変更履歴](CHANGELOG_ja.md)
