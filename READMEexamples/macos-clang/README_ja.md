# macOS / Clang ビルド

[English](README.md)

動作確認環境: macOS 27.0（Apple Silicon）、CMake 4.4.3、
Homebrew LLVM 23.1.1（libc++）、Ninja 1.13.2。
CMake 4.4.3 以降、LLVM Clang 21.1.8 以降、および同梱の
`clang-scan-deps` と C++23 の `std` モジュールが必要です。

ビルドツールをインストールします。

```sh
brew install cmake ninja llvm
```

このディレクトリで次のコマンドを実行します。

```sh
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel
./build/debugtrace-example
```

CMake は `/opt/homebrew/opt/llvm`（Apple Silicon）または
`/usr/local/opt/llvm`（Intel）の Homebrew LLVM を自動選択します。
このサンプルでは `/usr/bin/clang++` の Apple Clang は使用できません。
環境変数 `CXX` が設定されている場合や別の LLVM を使用する場合は、
新しいビルドディレクトリでコンパイラーを明示します。

```sh
cmake -S . -B build/llvm -G Ninja \
  -DCMAKE_CXX_COMPILER="$(brew --prefix llvm)/bin/clang++" \
  -DCMAKE_BUILD_TYPE=Debug
cmake --build build/llvm --parallel
./build/llvm/debugtrace-example
```

リリースビルドでは `-DCMAKE_BUILD_TYPE=Release` を指定します。
ビルド成果物のクリーンには `cmake --build build --target clean` を使用します。

`src/readme_example.cpp`、`src/point.cppm`、`../../src/debugtrace.cppm` を
C++23 でコンパイルします。標準ライブラリの `std` モジュールも CMake がビルドし、
モジュールの依存関係に従ってコンパイル順序を管理します。
LLVM コンパイラーの配置先から `lib/c++/libc++.modules.json` を検出し、
同じ LLVM の libc++ ヘッダーと実行時ライブラリを使用します。
実行ファイルに rpath を設定するため、`DYLD_LIBRARY_PATH` の設定は不要です。

CMake の [`import std;` 対応](https://cmake.org/cmake/help/v4.4/prop_tgt/CXX_MODULE_STD.html)
は実験的機能のため、構成時に警告が表示されます。
標準ライブラリのビルド時には Clang が `-Wreserved-module-identifier` の警告を
表示する場合もあります。
`CMakeLists.txt` の有効化 UUID は CMake 4.4.3 用です。CMake を更新する場合は、
[使用するバージョンの実験的機能の説明](https://github.com/Kitware/CMake/blob/v4.4.3/Help/dev/experimental.rst#c-import-std-support)
を確認してください。
