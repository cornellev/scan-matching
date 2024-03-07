// Copyright (C) 2024 Ethan Uppal. All rights reserved.

#pragma once

#define WINDOW_WIDTH 700
#define WINDOW_HEIGHT 700

namespace sim_config {
    // Number of points
    constexpr int n = 40;

    // Spacing of points
    constexpr double scale = 10;

    // Wall parameters
    constexpr double slope = -1.0;
    constexpr double intercept = 0.0;

    // Make wall slightly shaky
    constexpr double perturbation_range = 10.0;
    constexpr double x_displace = 100;
    constexpr double y_displace = 50;

    // Centering
    constexpr double x_unshift = n * scale / 2;
    constexpr double y_unshift = slope * x_unshift;
}
