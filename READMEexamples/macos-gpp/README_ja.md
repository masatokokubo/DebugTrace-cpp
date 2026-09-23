# macOS / GNU g++ ビルド

[English](README.md)

機能上の最小要件: CMake 4.0、g++ 15.1、Ninja 1.11。
最小バージョンの組み合わせでの動作は未検証です。

macOS arm64 / CMake 4.4.3 / Homebrew g++ 16.2.0 / Ninja 1.13.2 で
ビルドと実行を確認しました。
g++ に対応する libstdc++ の標準ライブラリモジュールが必要です。

Xcode Command Line Tools がなければ `xcode-select --install` で導入し、
Homebrew でビルドツールをインストールします。

```sh
brew install cmake gcc ninja
```

macOS の `/usr/bin/g++` は Apple Clang です。このプロジェクトは
`g++-16`、`g++-15` を自動検索します。Apple Silicon / Intel Mac の
Homebrew 標準配置にも対応しています。別の GNU g++ を使用する場合は、
構成時に `-DCMAKE_CXX_COMPILER=/path/to/g++` を指定してください。
明示した CMake コンパイラー、環境変数 `CXX`、ツールチェーンファイルが優先されます。

このディレクトリで次のコマンドを実行します。

```sh
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel
./build/debugtrace-example
```

既存の `build` が別のコンパイラーで構成されている場合は、3つのコマンドすべてで
新しいビルドディレクトリ（例: `build-gcc`）を指定してください。
CMake が C++ モジュールの依存関係を管理するために Ninja が必要です。

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
