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
            <a href="https://github.com/cornellev/scan-matching">https://github.com/cornellev/scan-matching</a>
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

- Interactive (graphical) visualization on point clouds
    - You can supply custom point clouds in a config file using fields from [`sensor_msgs::LaserScan`](http://docs.ros.org/en/api/sensor_msgs/html/msg/LaserScan.html).
- ICP library for external usage

\section install_sec Usage

\subsection install_unix_subsec Unix-like (MacOS, Linux)

First, download and install the dependencies.

- [SDL](https://www.libsdl.org)
- My [SDL wrapper](https://github.com/cornellev/sdl-wrapper)
- [libcmdapp2](https://ethanuppal.com/libcmdapp2/)
- [libconfig](https://github.com/ethanuppal/config)
- [eigen3](http://eigen.tuxfamily.org/index.php?title=Main_Page)

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
./main --gui
```
Instructions are printed to standard output.

The program itself can be built with
```shell
make
```
which will create an executable named `main` in the working directory.

\subsection install_windows_subsec Windows

\section example_sec Example
