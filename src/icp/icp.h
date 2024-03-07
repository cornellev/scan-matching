// Copyright (C) 2024 Ethan Uppal. All rights reserved.

#pragma once

#include <functional>

namespace icp {
    struct Point {
        double x;
        double y;

        Point(): x(0), y(0) {}
        Point(double x, double y): x(x), y(y) {}
    };

    struct Transform {
        double dx;
        double dy;

        Transform(): dx(0), dy(0) {}
        Transform(double dx, double dy): dx(dx), dy(dy) {}
    };

    class ICP {
        /** The source point cloud. */
        const std::vector<icp::Point> a;

        /** The destination point cloud. */
        const std::vector<icp::Point> b;

        /** The learning rate at which optimization is done. */
        const double rate;

        /** The pairing of each point in `a` to its closest in `b`. */
        std::vector<size_t> pair;

        /** The distance from each point in `a` to its closest in `b`. */
        std::vector<double> dist;

        /** The current point cloud transformation that is being optimized. */
        Transform t;

        /** Keeps track of the previous cost to ensure that progress is being
         * made. @see ICP::current_cost. */
        double previous_cost = INFINITY;

        /** The sum-of-squares cost of `dist`. */
        double current_cost = INFINITY;

    public:
        /** Constructs an instance of iterative closest points for `a` and `b`
         * progressing at `rate`. */
        ICP(const std::vector<icp::Point>& a, const std::vector<icp::Point>& b,
            double rate);

        /** Perform one iteration of ICP. */
        void iterate();

        /** Perform ICP until the cost is below `convergence_threshold` or until
         * no progress is being made. */
        void converge(double convergence_threshold);

        /** The source point cloud. */
        const std::vector<icp::Point>& source() const;

        /** The destination point cloud. */
        const std::vector<icp::Point>& destination() const;

        /** The current optimal cost. */
        double cost() const;

        /** The current optimal transform. */
        const Transform& transform() const;
    };
}
