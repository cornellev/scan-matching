# scan-matching

> [!NOTE]
> Please read [cornellev.github.io/scan-matching/](https://cornellev.github.io/scan-matching/) for the most up-to-date documentation.

## Introduction

This repository hosts Ethan's (my) implementation of Iterative Closest Points (ICP) as applied to scan matching.
It is a first step in my attempt to implement Simultaneous Localization and Mapping (SLAM).
Eventually, I will factor this out into its own library as a submodule for the SLAM library I will build.

## Usage

> [!NOTE]
> This section is for Unix-like systems. 
> I have not yet tested installation on Windows.
> Notably, the `make install` scripts rely on `/usr/local/lib`.

First, download and install the dependencies.

- [SDL](https://www.libsdl.org)
- My [SDL wrapper](https://github.com/cornellev/sdl-wrapper)
- [libcmdapp2](https://ethanuppal.com/libcmdapp2/)
- [libconfig](https://github.com/ethanuppal/config)
- [eigen3](http://eigen.tuxfamily.org/index.php?title=Main_Page)

There is also a dependency on [simple-test](https://github.com/ethanuppal/simple-test) if you want to run the tests (`make test`).

It is likely that you already have SDL installed.
If not, follow the instructions at the link provided (which goes to the SDL website).
Then, to download and install the remaining dependencies, run

```shell
# install libcmdapp
git clone https://github.com/ethanuppal/libcmdapp2.git
cd libcmdapp2
sudo make install
cd ..

# install libconfig
git clone https://github.com/ethanuppal/config.git
cd config
sudo make install
cd ..
```

Now, run
```shell
git clone https://github.com/cornellev/scan-matching.git
cd scan-matching
make
./main -S ex_data/scan1.conf -D ex_data/scan2.conf
```
Instructions are printed to standard output.

The program itself can be built with
```shell
make
```
which will create an executable named `main` in the working directory.
