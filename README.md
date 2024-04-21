<!-- THIS FILE IS GENERATED AUTOMATICALLY. -->
<!-- DO NOT EDIT THIS FILE. -->
<!-- EDIT README.md.build INSTEAD. -->
# scan-matching

> Last updated 2024-04-21 00:20:31.773084

## Introduction

This repository hosts Ethan's (my) implementation of Iterative Closest Points (ICP) as applied to scan matching.
It is a first step in my attempt to implement Simultaneous Localization and Mapping (SLAM).
Eventually, I will factor this out into its own library as a submodule for the SLAM library I will build.

## Install

You can view installation instructions at [INSTALL.md](INSTALL.md).

## Usage and Documentation

I host the usage information and documentation at [cornellev.github.io/scan-matching/](https://cornellev.github.io/scan-matching/).
Please see there for information on how to download and how to use the library.

You can build the documentation yourself locally with `make docs`.
The main page will be located at `docs/index.html` relative to the project root.

## v1.1.1 

The algorithm runs extremely fast now.
We only need it to run at 6hz with our current LiDAR.

![](book/asset/img/v1.1.1bench.png)

It also matches quite well.
Below is the result of running this ICP implementation on two point clouds obtained within the workspace.

![](book/asset/img/v1.1.1result.png)

However, there is still remove for improvement with regard to outlier rejection and other parts of the algorithm (for instance, adopting a point-to-line metric).

