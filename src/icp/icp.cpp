// Copyright (C) 2024 Ethan Uppal. All rights reserved.

#include <cmath>
#include "icp.h"

namespace icp {
    ICP::ICP(size_t n, double rate): rate(rate), pair(n), dist(n) {}

    void ICP::set_initial(Transform t) {
        this->t = t;
    }

    void ICP::converge(const std::vector<icp::Point>& a,
        const std::vector<icp::Point>& b, double convergence_threshold) {
        while (current_cost > convergence_threshold
               && (current_cost < previous_cost || current_cost == INFINITY)) {
            iterate(a, b);
        }
    }

    double ICP::cost() const {
        return current_cost;
    }

    const Transform& ICP::transform() const {
        return t;
    }
}
