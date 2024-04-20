// Copyright (C) 2024 Ethan Uppal. All rights reserved.

#include <cmath>
#include "sim_config.h"

namespace sim_config {
    int window_width = 700;
    int window_height = 700;

    int n = 100;

    double scale = 5;

    double slope = -1.0;
    double intercept = 0.0;

    double perturbation_range = 10.0;

    double x_displace = 100;
    double y_displace = 50;
    double angle_displace = M_PI_4;

    double x_delta = 0;

    bool use_light_background = false;
}
