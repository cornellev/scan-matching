// Copyright (C) 2024 Ethan Uppal. All rights reserved.

#include <cassert>
#include "../icp.h"

namespace icp {
    struct PointToLine_old final : public ICP {
        PointToLine_old(size_t n, double rate): ICP(n, rate) {}

        void iterate(const std::vector<icp::Vector>& a,
            const std::vector<icp::Vector>& b) override {
            // Use https://ieeexplore.ieee.org/document/4543181/authors

            // size_t n = a.size();
            // size_t m = b.size();

            // // point-to-point matching O(n^2)
            // for (size_t i = 0; i < n; i++) {
            //     dist[i] = INFINITY;
            //     for (size_t j = 0; j < m; j++) {
            //         double alt = std::hypot(a[i][0] - b[j][0], a[i][1] -
            //         b[j][1]); if (alt < dist[i]) {
            //             pair[i] = j;
            //             dist[i] = alt;
            //         }
            //     }
            // }

            // // gradient descent
            // double dx_effect = 0;
            // double dy_effect = 0;
            // for (size_t i = 0; i < n; i++) {
            //     // replace with gradient of L
            //     dx_effect += (a[i][0] + t.dx - b[pair[i]][0]) * 2 / n;
            //     dy_effect += (a[i][1] + t.dy - b[pair[i]][1]) * 2 / n;
            // }
            // t.dx -= dx_effect * rate;
            // t.dy -= dy_effect * rate;

            // // compute new cost
            // previous_cost = current_cost;
            // current_cost = 0;
            // for (size_t i = 0; i < n; i++) {
            //     // replace with your L
            //     double cx = a[i][0] + t.dx - b[pair[i]][0];
            //     double cy = a[i][1] + t.dy - b[pair[i]][1];
            //     current_cost += (cx * cx + cy * cy) / n;
            // }
        }
    };

    static bool static_initialization = []() {
        assert(ICP::register_method("point_to_line_old",
            [](size_t n, double rate) -> std::unique_ptr<ICP> {
                return std::make_unique<PointToLine_old>(n, rate);
            }));
        return true;
    }();
}
