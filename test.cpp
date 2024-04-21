// Copyright (C) 2024 Ethan Uppal. All rights reserved.

extern "C" {
#include <simple_test/simple_test.h>
}

#include "icp/icp.h"

void test_kdtree(void) {}

void test_icp(void) {
    std::unique_ptr<icp::ICP> icp = icp::ICP::from_method("vanilla");
    {
        std::vector<icp::Vector> a = {icp::Vector(0, 0)};
        std::vector<icp::Vector> b = {icp::Vector(100, 0)};
        icp->begin(a, b, icp::RBTransform());
        icp::ICP::ConvergenceReport result = icp->converge(10, 10);

        // should not need more than 10 for such a trivial situation or else
        // there is a serious issue with the algorithm
        assert_equal(10, result.iteration_count);

        // again, for such a trivial situation, we should have easily achieved
        // the convergence requested
        assert_true(result.final_cost < 10);
    }
}

void test_main() {
    test_kdtree();
    test_icp();
}
