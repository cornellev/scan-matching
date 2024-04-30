// Copyright (C) 2024 Ethan Uppal. All rights reserved.

extern "C" {
#include <simple_test/simple_test.h>
}

#include "icp/icp.h"

void test_kdtree(void) {}

void test_icp(const char* method) {
    std::unique_ptr<icp::ICP> icp = icp::ICP::from_method(method);

    {
        std::vector<icp::Vector> a = {icp::Vector(0, 0)};
        std::vector<icp::Vector> b = {icp::Vector(100, 0)};
        icp->begin(a, b, icp::RBTransform());
        icp::ICP::ConvergenceReport result = icp->converge(0, 0);

        // should not need more than 10 for such a trivial situation or else
        // there is a serious issue with the algorithm
        assert_true(result.iteration_count <= 10);

        // again, for such a trivial situation, we should have easily achieved
        // the convergence requested
        assert_true(result.final_cost == 0);

        assert_true(fabs(icp->current_transform().translation.y() - 0) <= 1e-6);
        assert_true(fabs(icp->current_transform().translation.x() - 100)
                    <= 1e-6);
    }

    {
        std::vector<icp::Vector> a = {icp::Vector(0, 0), icp::Vector(100, 100)};
        std::vector<icp::Vector> b = {icp::Vector(0, 100), icp::Vector(100, 0)};
        icp->begin(a, b, icp::RBTransform());
        icp::ICP::ConvergenceReport result = icp->converge(0, 0);

        assert_true(result.iteration_count <= 10);
        assert_true(result.final_cost <= 10);

        assert_true(fabs(icp->current_transform().translation.y() - 0) <= 1e-6);
        assert_true(fabs(icp->current_transform().translation.x() - 0) <= 1e-6);
    }

    {
        std::vector<icp::Vector> a = {icp::Vector(0, 0), icp::Vector(0, 100)};
        std::vector<icp::Vector> b = {
            icp::Vector(100, 0), icp::Vector(100, 100)};
        icp->begin(a, b, icp::RBTransform());
        icp::ICP::ConvergenceReport result = icp->converge(0, 0);

        assert_true(result.iteration_count <= 10);
        assert_true(result.final_cost <= 10);
        assert_true(fabs(icp->current_transform().translation.y() - 0) <= 1e-6);
        assert_true(fabs(icp->current_transform().translation.x() - 100)
                    <= 1e-6);
    }
}

void test_main() {
    test_kdtree();
    test_icp("vanilla");
}
