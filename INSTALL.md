# Installation Instructions

This document assumes you have downloaded the repository and installed the library dependencies.
If not, please read the instructions at [cornellev.github.io/scan-matching/](https://cornellev.github.io/scan-matching/).

## System Installation (Static)

> **Warning**: these instructions must be followed from start whenever an update to the installation is desired.

1. Run `make libcevicp.a` from the project root directory to build the static library `libcevicp.a`.
2. Navigate to the `install/` directory.
3. Run `sudo make install` to install the library.
    You can specify the installation locations by passing the `LIB` and `HEADER` variables.
    The default behavior is
    ```
    sudo make install LIB=/usr/local/lib HEADER=/usr/local/include
    ```
    The library `libcevicp.a` will be located at `LIB`.
    The header files will be accessible from `HEADER/cev_icp`, e.g., `HEADER/cev_icp/icp/icp.h`.
4.  Run `sudo make uninstall` to remove the library.
    You can similarly pass the `LIB` and `HEADER` variables, but they must be the same as where you installed.

## Local Installation (Static)

Follow the same instructions as above, but supply `LIB` and `HEADER` to be the desired local paths.

## Git Submodule

If you choose to include this as a git submodule, you must take care to only include the following files in your build system and include path:

- `src/algo/`
- `src/icp/`
