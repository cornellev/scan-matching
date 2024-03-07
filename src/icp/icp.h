// Copyright (C) 2024 Ethan Uppal. All rights reserved.

#pragma once

#include <cmath>
#include <vector>
#include <memory>

namespace icp {
    struct Point final {
        double x;
        double y;

        Point(): x(0), y(0) {}
        Point(double x, double y): x(x), y(y) {}
    };

    struct Transform final {
        double dx;
        double dy;

        Transform(): dx(0), dy(0) {}
        Transform(double dx, double dy): dx(dx), dy(dy) {}
    };

    class ICP {
    protected:
        /** The rate at which optimization is done. */
        const double rate;

        /** The current point cloud transformation that is being optimized. */
        Transform t;

        /** Keeps track of the previous cost to ensure that progress is being
         * made. @see ICP::current_cost. */
        double previous_cost = INFINITY;

        /** The sum-of-squares cost of `dist`. */
        double current_cost = INFINITY;

        /** The pairing of each point in `a` to its closest in `b`. */
        std::vector<size_t> pair;

        /** The distance metric of each point in `a`. */
        std::vector<double> dist;

        /** A new ICP instance for `n`-point matching and an optimization rate
         * of `rate` */
        ICP(size_t n, double rate);

    public:
        /** Sets an initial guess for the transform. */
        void set_initial(Transform t);

        /** Perform one iteration of ICP for point clouds `a` and `b`. */
        virtual void iterate(const std::vector<icp::Point>& a,
            const std::vector<icp::Point>& b) = 0;

        /** Perform ICP for point clouds `b` and `b` until the cost is below
         * `convergence_threshold` or until no progress is being made. */
        void converge(const std::vector<icp::Point>& a,
            const std::vector<icp::Point>& b, double convergence_threshold);

        /** The current optimal cost. */
        double cost() const;

        /** The current optimal transform. */
        const Transform& transform() const;

        /** Point-to-point ICP. */
        static std::unique_ptr<ICP> point_to_point(size_t n, double rate);
    };
}
