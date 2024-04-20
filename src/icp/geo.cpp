// Copyright (C) 2024 Ethan Uppal. All rights reserved.

#include "geo.h"

namespace icp {
    Point get_center_of_mass(const std::vector<Point>& points) {
        Point center_of_mass{};
        for (const Point& point: points) {
            center_of_mass.x += point.x;
            center_of_mass.y += point.y;
        }
        center_of_mass.x /= points.size();
        center_of_mass.y /= points.size();
        return center_of_mass;
    }
}
