\mainpage Introduction

\section overview_sec Overview

Scan-matching is a method for localization that uses high-frequency LiDAR data.
This project is [Ethan](https://ethanuppal.com)'s implementation of algorithms for scan-matching.

<table>
    <tr>
        <th style="text-align:left; vertical-align:top">Item</th>
        <th style="text-align:left; vertical-align:top">Location</th>
    </tr>
    <tr>
        <td style="text-align:left; vertical-align:top"><b>Documentation</b></td>
        <td style="text-align:left; vertical-align:top">icp::ICP</td>
    </tr>
    <tr>
        <td style="text-align:left; vertical-align:top"><b>Repository</b></td>
        <td style="text-align:left; vertical-align:top">
            <a href="https://github.com/cornellev/icp">https://github.com/cornellev/icp</a>
        </td>
    </tr>
    <tr>
        <td style="text-align:left; vertical-align:top"><b>Contents</b></td>
        <td style="text-align:left; vertical-align:top">
            \ref overview_sec <br> 
            \ref feature_sec <br> 
            \ref install_sec <br> 
            \ref example_sec
        </td>
    </tr>
</table>

\section feature_sec Features

- Interactive (graphical) visualization of point clouds
    - You can supply custom point clouds in a config file using fields from [`sensor_msgs::LaserScan`](http://docs.ros.org/en/api/sensor_msgs/html/msg/LaserScan.html).
- ICP library for external usage

\section install_sec Usage

\warning This section applies only to Unix-like operating systems, such as MacOS and Linux.

First, download and install the dependencies.
**Only eigen3 is necessary for the library. If you only wish to build the library, install eigen3 only. The remaining dependencies are only for the visualization tool.**

| Dependency | Library Location (at which) | Header Location (under which) |
| --- | --- | --- |
| [eigen3](http://eigen.tuxfamily.org/index.php?title=Main_Page) | N/A | `/usr/local/include/eigen3/` |
| [SDL2](https://www.libsdl.org) | `$(sdl2-config --cflags)` | `$(sdl2-config --libs)` |
| My [SDL2 wrapper](https://github.com/cornellev/sdl-wrapper) | `/usr/local/lib/libsdlwrapper.a` | `/usr/local/include/sdlwrapper/` |
| [libcmdapp2](https://ethanuppal.com/libcmdapp2/) | `/usr/local/lib/libcmdapp.a` | `/usr/local/include/` |
| [libconfig](https://github.com/ethanuppal/config) | `/usr/local/lib/libconfig.a` | `/usr/local/include/` |

There is also a dependency on [simple-test](https://github.com/ethanuppal/simple-test) if you want to run the tests (`make test`).
Please follow the installation instructions there.

It is likely that you already have SDL installed.
If not, follow the instructions at the link provided (which goes to the SDL website).
Do the same for eigen3.
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

Then, a simple clone should suffice.
```shell
git clone https://github.com/cornellev/icp.git
cd icp
```

\subsection library_sec Library

To use the library, read the [INSTALL.md](https://github.com/cornellev/icp/blob/main/INSTALL.md
) document in the repository.
Read \ref write_icp_instance to use your own ICP implementations.

\subsection vis_tool_sec Visualization & Benchmarking Tool

The following command visualizes the two LiDAR scans at the given files.
Instructions are printed to standard output.

```shell
make
./main \
    -S ex_data/scan1/first.conf \
    -D ex_data/scan1/second.conf \
    --method vanilla \
    --gui
```

You can benchmark with `make bench`; by default, this will pass `-mvanilla`.

The program itself can be built with
```shell
make
```
which will create an executable named `main` in the working directory.
