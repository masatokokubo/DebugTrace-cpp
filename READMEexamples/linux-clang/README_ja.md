# Linux / Clang ビルド

[English](README.md)

動作確認環境: CMake 4.4.3、Clang 21.1.8、Ninja 1.11.1、
GCC Toolset 15 の libstdc++。
Clang と同じバージョンの `clang-scan-deps`、および C++23 の `std` モジュールを
提供する標準ライブラリが必要です。

このディレクトリで次のコマンドを実行します。

```sh
cmake -S . -B build -G Ninja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel
./build/debugtrace-example
```

リリースビルドに切り替える場合は、構成コマンドの
`-DCMAKE_BUILD_TYPE=Debug` を `-DCMAKE_BUILD_TYPE=Release` に変更します。
ビルド成果物のクリーンには `cmake --build build --target clean` を使用します。

`src/readme_example.cpp`、`src/point.cppm`、`../../src/debugtrace.cppm` を
C++23 でコンパイルします。標準ライブラリの `std` モジュールも CMake がビルドし、
モジュールの依存関係に従ってコンパイル順序を管理します。

標準ライブラリには Clang の既定設定を使用します。動作確認環境では
`clang++ -print-file-name=libstdc++.modules.json` で GCC Toolset 15 の
モジュール情報ファイルが見つかる設定になっています。

CMake 4.4.3 の [`import std;` 対応](https://cmake.org/cmake/help/v4.4/prop_tgt/CXX_MODULE_STD.html)
は実験的機能のため、構成時に警告が表示されます。
また、この環境では標準ライブラリのビルド時に Clang が
`-Wreserved-module-identifier` の警告を表示しますが、ビルドと実行は成功します。
`CMakeLists.txt` の有効化 UUID は CMake 4.4.3 用です。CMake を更新する場合は、
[使用するバージョンの実験的機能の説明](https://github.com/Kitware/CMake/blob/v4.4.3/Help/dev/experimental.rst#c-import-std-support)
を確認してください。
