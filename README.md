# scan-matching

## Introduction

This repository hosts Ethan's (my) implementation of Iterative Closest Points (ICP) as applied to scan matching.
It is a first step in my attempt to implement Simultaneous Localization and Mapping (SLAM).
Eventually, I will factor this out into its own library as a submodule for the SLAM library I will build.

## Dependencies

- [SDL](https://www.libsdl.org)
- My [SDL wrapper](https://github.com/cornellev/sdl-wrapper)
- [libcmdapp2](http://ethanuppal.com/libcmdapp2/)

## Usage

You can launch the interactive window simply by running
```bash
make run 
```
Instructions are printed to standard output.

The program itself can be built with
```bash
make
```
which will create an executable named `./main.out` and a script `./main` which handles dynamic library dependencies.
For now, this solution is suboptimal becaues `./main` can only be run from the project root directly.
Use `./main --help` to see usage information.
