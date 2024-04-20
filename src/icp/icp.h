// Copyright (C) 2024 Ethan Uppal. All rights reserved.

#pragma once

#include <cmath>
#include <vector>
#include <memory>
#include <string>
#include <functional>
#include "geo.h"

namespace icp {
    /**
     * Interface for iterative closest points; please read the Detailed
     * Description for usage information.
     *
     * \par Example
     * @code
     * // Construct a new vanilla ICP instance.
     * std::unique_ptr<icp::ICP> icp = icp::ICP::from_method("vanilla");
     * @endcode
     *
     * \par Usage
     * Let `a` and `b` be two point clouds of type `std::vector<icp::Point>`.
     * Then, given an ICP instance `icp` of type `std::unique_ptr<icp::ICP>`,
     * perform the following steps.
     *
     * 1. Call `icp->begin(a, b, initial_guess)`.
     * 2. Call either `icp->converge(a, b, convergence_threshold)` or repeatedly
     * `icp->iterate(a, b)`.
     *
     * If these steps are not followed as described here, the behavior is
     * undefined.
     *
     * At any point in the process, you may query `icp->cost()` and
     * `icp->transform()`.
     */
    class ICP {
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
         * of `rate`. @deprecated `n` is ignored. */
        ICP(size_t n = 0, double rate = 0.01);

        ICP(double rate = 0.01);

        virtual void setup(const std::vector<icp::Point>& a,
            const std::vector<icp::Point>& b);

    public:
        virtual ~ICP() = default;

        /** Begins the ICP process for point clouds `a` and `b` with an initial
         * guess for the transform `t`. */
        void begin(const std::vector<icp::Point>& a,
            const std::vector<icp::Point>& b, Transform t);

        /** Perform one iteration of ICP for point clouds `a` and `b`. */
        virtual void iterate(const std::vector<icp::Point>& a,
            const std::vector<icp::Point>& b) = 0;

        /** Perform ICP for point clouds `a` and `b` until the cost is below
         * `convergence_threshold` or until no progress is being made. */
        void converge(const std::vector<icp::Point>& a,
            const std::vector<icp::Point>& b, double convergence_threshold);

        /** The current cost. */
        double cost() const;

        /** The current transform. */
        const Transform& transform() const;

        /** Registers a new ICP method that can be created with `constructor`,
         * returning `false` if `name` has already been registered. */
        static bool register_method(std::string name,
            std::function<std::unique_ptr<ICP>(size_t n, double rate)>
                constructor);

        /** Returns a current list of the names of currently registered ICP
         * methods. */
        static const std::vector<std::string>& registered_methods();

        /**
         * Factory constructor for the ICP method `name`. @pre `name` is a
         * valid registered method.
         *
         * @deprecated `n` is ignored.
         */
        static std::unique_ptr<ICP> from_method(std::string name, size_t n = 0,
            double rate = 0.01);
    };

    struct Methods {
        std::vector<std::string> registered_method_names;
        std::vector<std::function<std::unique_ptr<ICP>(size_t n, double rate)>>
            registered_method_constructors;
    };
}
