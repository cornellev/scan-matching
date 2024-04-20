// Copyright (C) 2024 Ethan Uppal. All rights reserved.

#include "icp.h"
#include "util/logger.h"

namespace icp {
    static Methods* global;

    ICP::ICP(size_t n, double rate): rate(rate), pair(n), dist(n) {}

    ICP::ICP(double rate): rate(rate) {}

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

        // Cost is infinite
        previous_cost = std::numeric_limits<double>::infinity();
        current_cost = std::numeric_limits<double>::infinity();

        // Ensure arrays are the right size
        const size_t n = a.size();
        pair.reserve(n);
        dist.reserve(n);

        // Per-instance customization routine
        setup();
    }

    void ICP::converge(double convergence_threshold) {
        while (current_cost > convergence_threshold
               && (current_cost < previous_cost || current_cost == INFINITY)) {
            previous_cost = current_cost;
            iterate();
        }
    }

    double ICP::cost() const {
        return current_cost;
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
        std::function<std::unique_ptr<ICP>(size_t, double)> constructor) {
        ensure_methods_exists();
        global->registered_method_constructors.push_back(constructor);
        global->registered_method_names.push_back(name);
        return true;
    }

    const std::vector<std::string>& ICP::registered_methods() {
        ensure_methods_exists();
        return global->registered_method_names;
    }

    std::unique_ptr<ICP> ICP::from_method(std::string name, size_t n,
        double rate) {
        ensure_methods_exists();
        size_t index = std::find(global->registered_method_names.begin(),
                           global->registered_method_names.end(), name)
                       - global->registered_method_names.begin();
        return global->registered_method_constructors[index](n, rate);
    }
}
