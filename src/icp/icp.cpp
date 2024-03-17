// Copyright (C) 2024 Ethan Uppal. All rights reserved.

#include "icp.h"
#include "util/logger.h"

namespace icp {
    static Methods* global;

    ICP::ICP(size_t n, double rate): rate(rate), pair(n), dist(n) {
        set_initial(Transform());
    }

    void ICP::set_initial(Transform t) {
        this->t = t;
        previous_cost = std::numeric_limits<double>::infinity();
        current_cost = std::numeric_limits<double>::infinity();
    }

    void ICP::converge(const std::vector<icp::Point>& a,
        const std::vector<icp::Point>& b, double convergence_threshold) {
        while (current_cost > convergence_threshold
               && (current_cost < previous_cost || current_cost == INFINITY)) {
            previous_cost = current_cost;
            iterate(a, b);
        }
    }

    double ICP::cost() const {
        return current_cost;
    }

    const Transform& ICP::transform() const {
        return t;
    }

    static void ensure_methods_exists() {
        if (!global) {
            global = new Methods();
        }
    }

    bool ICP::register_method(std::string name,
        std::function<std::unique_ptr<ICP>(size_t, double)> constructor) {
        ensure_methods_exists();
        Log << "<ICP> register_method '" << name << "'\n";
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
