# Linux / g++ ビルド

[English](README.md)

機能上の最小要件: CMake 4.0、g++ 15.1、Ninja 1.11。
最小バージョンの組み合わせでの動作は未検証です。

動作確認環境: CMake 4.4.3、g++ 15.2.1、Ninja 1.11.1。
g++ に対応する libstdc++ の標準ライブラリモジュールが必要です。

このディレクトリで次のコマンドを実行します。

```sh
cmake -S . -B build -G Ninja -DCMAKE_CXX_COMPILER=g++ -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel
./build/debugtrace-example
```

リリースビルドに切り替える場合は、構成コマンドの
`-DCMAKE_BUILD_TYPE=Debug` を `-DCMAKE_BUILD_TYPE=Release` に変更します。
ビルド成果物のクリーンには `cmake --build build --target clean` を使用します。

`src/readme_example.cpp`、`src/point.cppm`、`../../src/debugtrace.cppm` を
C++23 でコンパイルします。標準ライブラリの `std` モジュールも CMake がビルドし、
モジュールの依存関係に従ってコンパイル順序を管理します。

CMake の `import std;` 対応は実験的機能のため、構成時に警告が表示されます。
`CMakeLists.txt` は CMake 4.0～4.4 のバージョンに応じて有効化 UUID を選択します。
それ以降の CMake を使用する場合は、
[使用するバージョンの実験的機能の説明](https://github.com/Kitware/CMake/blob/v4.4.3/Help/dev/experimental.rst#c-import-std-support)
を確認してください。
