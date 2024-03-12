// Copyright (C) 2024 Ethan Uppal. All rights reserved.

#include "icp.h"
#include <iostream>
namespace icp {
    struct PointToLine final : public ICP {
        PointToLine(size_t n, double rate): ICP(n, rate) {}

        void iterate(const std::vector<icp::Point>& a,
            const std::vector<icp::Point>& b) override {
            size_t n = a.size();
            size_t m = b.size();

            // compute center of mass of a
            double a_cm_x = 0;
            double a_cm_y = 0;
            for (size_t i = 0; i < n; i++) {
                a_cm_x += a[i].x;
                a_cm_y += a[i].y;
            }
            a_cm_x /= n;
            a_cm_y /= n;

            t.cx = a_cm_x;
            t.cy = a_cm_y;

            // point-to-point matching O(n^2)
            for (size_t i = 0; i < n; i++) {
                dist[i] = INFINITY;
                for (size_t j = 0; j < m; j++) {
                    double alt = std::hypot(a[i].x - b[j].x, a[i].y - b[j].y);
                    if (alt < dist[i]) {
                        pair[i] = j;
                        dist[i] = alt;
                    }
                }
            }

            // gradient descent
            double dx_effect = 0;
            double dy_effect = 0;
            double theta_effect = 0;
            for (size_t i = 0; i < n; i++) {
                // replace with gradient of L
                double o_x = (a[i].x - a_cm_x);
                double o_y = (a[i].y - a_cm_y);
                double a_rot_x = std::cos(t.theta) * o_x
                                 - std::sin(t.theta) * o_y + a_cm_x;
                double a_rot_y = std::sin(t.theta) * o_x
                                 + std::cos(t.theta) * o_y + a_cm_y;
                double b_x = b[pair[i]].x;
                double b_y = b[pair[i]].y;
                dx_effect += (a_rot_x + t.dx - b_x) * 2 / n;
                dy_effect += (a_rot_y + t.dy - b_y) * 2 / n;
                theta_effect += (b_x * o_x * std::sin(t.theta)
                                    + b_x * o_y * std::cos(t.theta)
                                    - b_y * o_x * std::cos(t.theta)
                                    + b_y * o_y * std::sin(t.theta)
                                    - a_cm_x * o_x * std::sin(t.theta)
                                    - a_cm_x * o_y * std::cos(t.theta)
                                    + a_cm_y * o_x * std::cos(t.theta)
                                    - a_cm_y * o_y * std::sin(t.theta)
                                    - o_x * t.dx * std::sin(t.theta)
                                    + o_x * t.dy * std::cos(t.theta)
                                    - o_y * t.dx * std::cos(t.theta)
                                    - o_y * t.dy * std::sin(t.theta))
                                * 2 / n;
            }
            t.dx -= dx_effect * rate;
            t.dy -= dy_effect * rate;
            t.theta -= theta_effect * rate;
            //            std::cout << dx_effect << ',' << dy_effect << ',' <<
            //            theta_effect
            // << '\n';
            // compute new cost
            previous_cost = current_cost;
            current_cost = 0;
            for (size_t i = 0; i < n; i++) {
                // replace with your L
                double o_x = (a[i].x - a_cm_x);
                double o_y = (a[i].y - a_cm_y);
                double a_rot_x = std::cos(t.theta) * o_x
                                 - std::sin(t.theta) * o_y + a_cm_x;
                double a_rot_y = std::sin(t.theta) * o_x
                                 + std::cos(t.theta) * o_y + a_cm_y;
                double cx = a_rot_x + t.dx - b[pair[i]].x;
                double cy = a_rot_y + t.dy - b[pair[i]].y;
                current_cost += (cx * cx + cy * cy) / n;
            }
        }
    };

    static bool static_initialization = []() {
        ICP::register_method("point_to_point",
            [](size_t n, double rate) -> std::unique_ptr<ICP> {
                return std::make_unique<PointToLine>(n, rate);
            });
        return true;
    }();
}
