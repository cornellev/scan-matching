// Copyright (C) 2024 Ethan Uppal. All rights reserved.

#include "geo.h"

namespace icp {
    Vector get_centroid(const std::vector<Vector>& points) {
        Vector center_of_mass{};
        for (const Vector& point: points) {
            center_of_mass[0] += point[0];
            center_of_mass[1] += point[1];
        }
        center_of_mass[0] /= points.size();
        center_of_mass[1] /= points.size();
        return center_of_mass;
    }
}
