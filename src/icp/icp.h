/*
 * @author Ethan Uppal
 * @copyright Copyright (C) 2024 Ethan Uppal. All rights reserved.
 */

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
     * Let `a` and `b` be two point clouds of type `std::vector<Vector>`.
     * Then, given an ICP instance `icp` of type `std::unique_ptr<icp::ICP>`,
     * perform the following steps.
     *
     * 1. Call `icp->begin(a, b, initial_guess)`.
     * 2. Call either `icp->converge(convergence_threshold)` or repeatedly
     * `icp->iterate()`.
     *
     * If these steps are not followed as described here, the behavior is
     * undefined.
     *
     * At any point in the process, you may query `icp->cost()` and
     * `icp->transform()`.
     */
    class ICP {
    protected:
        /** The current point cloud transformation that is being optimized. */
        RBTransform transform;

        /** The centroid of the source point cloud. */
        Vector a_cm;

        /** The centroid of the destination point cloud. */
        Vector b_cm;

        /** The source point cloud relative to its centroid. */
        std::vector<Vector> a;

        /** The destination point cloud relative to its centroid. */
        std::vector<Vector> b;

        /** Keeps track of the previous cost to ensure that progress is being
         * made. @see ICP::current_cost. */
        double previous_cost;

        /** The RMS (root mean square) cost of the current transformation. */
        double current_cost;

        /** The pairing of each point in `a` to its closest in `b`. */
        std::vector<size_t> pair;

        /** The distance metric of each point in `a`. */
        std::vector<double> dist;

        ICP();

        virtual void setup();

    public:
        struct ConvergenceReport {
            double final_cost;
            size_t iteration_count;
        };

        virtual ~ICP() = default;

        /** Begins the ICP process for point clouds `a` and `b` with an initial
         * guess for the transform `t`. */
        void begin(const std::vector<Vector>& a, const std::vector<Vector>& b,
            RBTransform t);

        /** Perform one iteration of ICP for the point clouds `a` and `b`
         * provided with ICP::begin. */
        virtual void iterate() = 0;

        /** Computes the cost of the current transformation. */
        double calculate_cost() const;

        /**
         * Perform ICP for the point clouds `a` and `b` provided with ICP::begin
         * until the cost is below `convergence_threshold` or until no progress
         * is being made. At least `burn_in` iterations will be performed.
         *
         * @returns Information about the convergence.
         */
        ConvergenceReport converge(size_t burn_in,
            double convergence_threshold);

        /** The current transform. */
        const RBTransform& current_transform() const;

        /** Registers a new ICP method that can be created with `constructor`,
         * returning `false` if `name` has already been registered. */
        static bool register_method(std::string name,
            std::function<std::unique_ptr<ICP>(void)> constructor);

        /** Returns a current list of the names of currently registered ICP
         * methods. */
        static const std::vector<std::string>& registered_methods();

        /**
         * Factory constructor for the ICP method `name`.
         *
         * @pre `name` is a valid registered method. See
         * ICP::is_registered_method.
         */
        static std::unique_ptr<ICP> from_method(std::string name);

        /** Whether `name` is a registered ICP method. */
        static bool is_registered_method(std::string name);
    };

    struct Methods {
        std::vector<std::string> registered_method_names;
        std::vector<std::function<std::unique_ptr<ICP>(void)>>
            registered_method_constructors;
    };
}
