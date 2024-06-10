# GRA Abschlussprojekt

## Authors

- Ivan Logvynenko
- (Please add your self)

## Build

### Requirements

* gcc/++ or clang/++
* CMake (v >= 3.5)
* Make

### Building

First you need to initialize build files with cmake
``` cmake
    cmake .
```
or if you want to keep working directory clean
``` cmake
    cmake <'build' or any other directory you like>
```
If you create 

now CMake should have generated Makefile in the spesified directory so you can run ```make``` to compile your code *(enter the directory you have generated files to)*

**Notice: CMake will automatically install systemc in *cmake/_deps/* and link it so you don't have to worry about anything**

