// Copyright (C) 2024 Ethan Uppal. All rights reserved.

#include "icp.h"
#include "util/logger.h"

namespace icp {
    std::vector<std::string> ICP::registered_method_names;
    std::unordered_map<std::string,
        std::function<std::unique_ptr<ICP>(size_t, double)>>*
        ICP::registered_method_constructors;

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

    bool ICP::register_method(std::string name,
        std::function<std::unique_ptr<ICP>(size_t, double)> constructor) {
        static std::unordered_map<std::string,
            std::function<std::unique_ptr<ICP>(size_t, double)>>
            local_registered_method_constructors;
        if (!registered_method_constructors) {
            registered_method_constructors =
                &local_registered_method_constructors;
        }
        if (registered_method_constructors->count(name)) {
            return false;
        }
        (*registered_method_constructors)[name] = constructor;
        registered_method_names.push_back(name);
        return true;
    }

    const std::vector<std::string>& ICP::registered_methods() {
        return registered_method_names;
    }

    std::unique_ptr<ICP> ICP::from_method(std::string name, size_t n,
        double rate) {
        return (*registered_method_constructors)[name](n, rate);
    }
}
