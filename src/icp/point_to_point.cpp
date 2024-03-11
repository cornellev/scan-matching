// Copyright (C) 2024 Ethan Uppal. All rights reserved.

#include "icp.h"

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
                double x_off = (a[i].x - a_cm_x);
                double y_off = (a[i].y - a_cm_y);
                double a_rot_x = std::cos(t.theta) * x_off
                                 - std::sin(t.theta) * y_off;
                double a_rot_y = std::sin(t.theta) * x_off
                                 + std::cos(t.theta) * y_off;
                double b_x = b[pair[i]].x;
                double b_y = b[pair[i]].y;
                dx_effect += (a_rot_x + t.dx - b_x) * 2 / n;
                dy_effect += (a_rot_y + t.dy - b_y) * 2 / n;
                theta_effect += (- std::sin(t.theta) * x_off * t.dx + std::cos(t.theta) * x_off * t.dy + std::sin(t.theta) * x_off * b_x - std::cos(t.theta) * y_off * t.dx + std::cos(t.theta) * y_off * b_y + std::cos(t.theta) * x_off * b_y - std::sin(t.theta) * y_off * t.dy + std::sin(t.theta) * y_off + b_y) * 2 / n;
            }
            t.dx -= dx_effect * rate;
            t.dy -= dy_effect * rate;
            t.theta -= theta_effect * rate;

            // compute new cost
            previous_cost = current_cost;
            current_cost = 0;
            for (size_t i = 0; i < n; i++) {
                // replace with your L
                double x_off = (a[i].x - a_cm_x);
                double y_off = (a[i].y - a_cm_y);
                double a_rot_x = std::cos(t.theta) * x_off
                                 - std::sin(t.theta) * y_off;
                double a_rot_y = std::sin(t.theta) * x_off
                                 + std::cos(t.theta) * y_off;
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
