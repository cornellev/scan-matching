// Copyright (C) 2024 Ethan Uppal. All rights reserved.

#pragma once

namespace sim_config {
    extern int window_width;
    extern int window_height;

    // Number of points
    extern int n;

    // Spacing of points
    extern double scale;

    // Wall parameters
    extern double slope;
    extern double intercept;

    // Make wall slightly shaky
    extern double perturbation_range;

    // Simulate having moved/turned
    extern double x_displace;
    extern double y_displace;
    extern double angle_displace;

    extern double x_delta;
}
