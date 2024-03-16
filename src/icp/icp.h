// Copyright (C) 2024 Ethan Uppal. All rights reserved.

#pragma once

#include <cmath>
#include <vector>
#include <memory>
#include <string>
#include <functional>

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
        double cx;
        double cy;

    public:
        Transform(): dx(0), dy(0), theta(0), cx(0), cy(0) {}
        Transform(double dx, double dy, double theta)
            : dx(dx), dy(dy), theta(theta) {}

        double transform_x(double x, double y) const {
            return std::cos(theta) * (x - cx) - std::sin(theta) * (y - cy) + cx
                   + dx;
        }
        double transform_y(double x, double y) const {
            return std::sin(theta) * (x - cx) + std::cos(theta) * (y - cy) + cy
                   + dy;
        }
    };

    class ICP {
        static std::vector<std::string> registered_method_names;
        static std::unordered_map<std::string,
            std::function<std::unique_ptr<ICP>(size_t n, double rate)>>*
            registered_method_constructors;

    protected:
        /** The rate at which optimization is done. */
        const double rate;

        /** The current point cloud transformation that is being optimized. */
        Transform t;

        /** Keeps track of the previous cost to ensure that progress is being
         * made. @see ICP::current_cost. */
        double previous_cost;

        /** The sum-of-squares cost of `dist`. */
        double current_cost;

        /** The pairing of each point in `a` to its closest in `b`. */
        std::vector<size_t> pair;

        /** The distance metric of each point in `a`. */
        std::vector<double> dist;

        /** A new ICP instance for `n`-point matching and an optimization rate
         * of `rate` */
        ICP(size_t n, double rate);

    public:
        ~ICP() = default;

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

        /** Registers a new ICP method that can be created with `constructor`,
         * returning `false` if `name` has already been registered. */
        static bool register_method(std::string name,
            std::function<std::unique_ptr<ICP>(size_t n, double rate)>
                constructor);

        /** Returns a current list of the names of currently registered ICP
         * methods. */
        static const std::vector<std::string>& registered_methods();

        /** Factory constructor for the ICP method `name`. @pre `name` is a
         * valid registered method. */
        static std::unique_ptr<ICP> from_method(std::string name, size_t n,
            double rate);
    };
}
