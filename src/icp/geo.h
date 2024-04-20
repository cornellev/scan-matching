// Copyright (C) 2024 Ethan Uppal. All rights reserved.

#pragma once

#include <vector>
#include <cmath>

namespace icp {
    struct Point final {
        double x;
        double y;

        Point() {}
        Point(double x, double y): x(x), y(y) {}
    };

    struct Transform final {
        double dx;
        double dy;
        double theta;  ///< currently only around CoM, need to make more
                       ///< complicated later
        Point cm;

    public:
        Transform(): dx(0), dy(0), theta(0), cm() {}
        Transform(double dx, double dy, double theta)
            : dx(dx), dy(dy), theta(theta) {}

        double transform_x(double x, double y) const {
            return std::cos(theta) * (x - cm.x) - std::sin(theta) * (y - cm.y)
                   + cm.x + dx;
        }
        double transform_y(double x, double y) const {
            return std::sin(theta) * (x - cm.x) + std::cos(theta) * (y - cm.y)
                   + cm.y + dy;
        }
    };

    Point get_center_of_mass(const std::vector<Point>& points);
}
