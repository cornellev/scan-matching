/*
 * @author Ethan Uppal
 * @copyright Copyright (C) 2024 Ethan Uppal. All rights reserved.
 */

#include <numeric>
#include "icp.h"
#include "util/logger.h"

namespace icp {
    static Methods* global;

    ICP::ICP() {}

    void ICP::setup() {}

    void ICP::begin(const std::vector<Vector>& a, const std::vector<Vector>& b,
        RBTransform t) {
        // Initial transform guess
        this->transform = t;

        // Copy in point clouds
        this->a = a;
        this->b = b;

        // Set relative to centroid
        a_cm = get_centroid(a);
        b_cm = get_centroid(b);
        for (Vector& point: this->a) {
            point -= a_cm;
        }
        for (Vector& point: this->b) {
            point -= b_cm;
        }

        // Cost is infinite initially
        previous_cost = std::numeric_limits<double>::infinity();
        current_cost = std::numeric_limits<double>::infinity();

        // Ensure arrays are the right size
        const size_t n = a.size();
        if (pair.size() < n) {
            pair.resize(n);
            dist.resize(n);
        }

        // Per-instance customization routine
        setup();
    }

    double ICP::calculate_cost() const {
        double sum_squares{};
        for (double value: dist) {
            sum_squares += value;
        }
        return std::sqrt(sum_squares / a.size());
    }

    ICP::ConvergenceReport ICP::converge(size_t burn_in,
        double convergence_threshold) {
        ConvergenceReport result{};

        // Repeat until convergence
        while (current_cost > convergence_threshold || current_cost == INFINITY
               || burn_in) {
            // Store previous iteration results
            previous_cost = current_cost;
            RBTransform previous_transform = transform;

            iterate();

            // If cost rose, revert to previous transformation/cost and
            // exit
            current_cost = calculate_cost();
            if (current_cost >= previous_cost && !burn_in) {
                transform = previous_transform;
                current_cost = previous_cost;
                break;
            }

            result.iteration_count++;
            burn_in--;
        }

        result.final_cost = current_cost;

        return result;
    }

    const RBTransform& ICP::current_transform() const {
        return transform;
    }

    static void ensure_methods_exists() {
        if (!global) {
            global = new Methods();
        }
    }

    bool ICP::register_method(std::string name,
        std::function<std::unique_ptr<ICP>(void)> constructor) {
        ensure_methods_exists();
        global->registered_method_constructors.push_back(constructor);
        global->registered_method_names.push_back(name);
        return true;
    }

    const std::vector<std::string>& ICP::registered_methods() {
        ensure_methods_exists();
        return global->registered_method_names;
    }

    std::unique_ptr<ICP> ICP::from_method(std::string name) {
        ensure_methods_exists();
        size_t index = std::find(global->registered_method_names.begin(),
                           global->registered_method_names.end(), name)
                       - global->registered_method_names.begin();
        return global->registered_method_constructors[index]();
    }

    bool ICP::is_registered_method(std::string name) {
        return std::find(global->registered_method_names.begin(),
                   global->registered_method_names.end(), name)
               != global->registered_method_names.end();
    }
}
