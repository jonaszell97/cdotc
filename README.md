<img src="https://imgur.com/eGguu7Y.png" alt="CDot Logo" height="70px" />

The C• Programming Language
===========================

| | **Architecture** | **Master** |
|---|:---:|:---:|
| **macOS**        | x86_64 |![Build Status](https://github.com/jonaszell97/cdotc/workflows/Build%20(macOS)/badge.svg)|
| **Ubuntu 18.04** | x86_64 |![Build Status](https://github.com/jonaszell97/cdotc/workflows/Build%20(Ubuntu%2018.04)/badge.svg)|
| **Ubuntu 16.04** | x86_64 |![Build Status](https://github.com/jonaszell97/cdotc/workflows/Build%20(Ubuntu%2016.04)/badge.svg)|

C• (pronouced _see-dot_) is a multi-paradigm systems programming language with the goal of combining the most popular features from a variety of modern languages. This includes (among many others)

- a **modern syntax**, inspired by Swift & Scala
- many of the convenience features provided by modern languages, such as **closures**, builtin **sum types** and easy **interoperability** with C-like languages
- automatic memory management via **reference counting**
- a (less restrictive) **ownership model**, inspired by Rust
- a powerful **macro system**, again inspired by Rust
- a variety of compile-time **metaprogramming** facilities
- a binary **module format** for distributing packages

C• binaries are compiled to machine code using LLVM, which means that the powerful optimization infrastructure of LLVM is automatically applied to every program you write.

For code examples and further documentation, please refer to the [docs](https://cdotc.jonaszell.dev/). 

- [Building cdotc](#building-cdotc)
  - [macOS](#macos)
  - [Linux](#linux)
  - [Building](#building)
- [Using cdotc](#using-cdotc)
  - [Building a standalone executable](#building-a-standalone-executable)
  - [Building a module](#building-a-module)

## Building cdotc

This repository contains the reference compiler for C•, also called cdotc. It is built using CMake.

### macOS

Building cdotc on macOS requires [CMake](https://cmake.org), [LLVM](https://llvm.org), [Python 3](https://python.org), and a C++17 compatible C++ compiler. All of these can be installed via [Homebrew](https://brew.sh/):

    brew install cmake llvm python

Additionally, installation of the XCode developer tools is recommended:

    xcode-select --install

### Linux

Building cdotc on Linux requires [CMake](https://cmake.org), [LLVM](https://llvm.org), [Python 3](https://python.org), and a C++17 compatible C++ compiler. All of these can be installed with the following command:

    sudo apt-get install cmake llvm g++ python

### Building

After installing the dependencies, building cdotc should be as simple as cloning this repository and executing the build script:

```
cd utils
./build_script --test
```

The `--test` option is optional, but executing tests is recommended to ensure your build works correctly.
If the build was successful, the `cdotc` binary should be included in your path and ready to use:

    cdotc --version
    cdotc version 0.0.1
    Optimizing compiler for the CDot language

Additionally, the C• standard library should have been installed to your `lib` directory (if the test suite passed you should not have to verify this).

## Using cdotc

### Building a standalone executable

Building a C• program with `cdotc` is easy, simply create a file with the extension `.dot` and pass the path to it as an argument.

    echo 'print("Hello, World!")' > main.dot
    cdotc main.dot

`cdotc` will treat this file as the entry point to your program, which means that you do not have to explicitly define a `main` function. Specifying additional files is possible, but the first file will always be considered as the application entry point.

Since no output name was specified, the resulting executable will be called `a.out` and can be run like this:

    ./a.out
    Hello, World!

### Building a module

C• provides a builtin module format that can be used to create distributable packages. To build a module, you pass a so-called _module file_ to `cdotc` instead of your sources. For a more detailed description of the module file format, refer to the documentation.

    cdotc /path/to/my/module/mod.dotm -emit-modules

This will create a binary module file in your `lib` directory which can then be referenced from other C• files using an `import` statement. 

