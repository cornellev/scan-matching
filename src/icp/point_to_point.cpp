// Copyright (C) 2024 Ethan Uppal. All rights reserved.

#include <cassert>
#include <cstdlib>
#include "icp.h"
#include "util/logger.h"

namespace icp {
    struct PointToPoint final : public ICP {
        int time{};
        double last_dx_effect{};
        double last_dy_effect{};
        double last_theta_effect{};
        double momentum_alpha = 0.02;
        double eta_decay = 1;

        PointToPoint(size_t n, double rate): ICP(n, rate) {}
        ~PointToPoint() override {}

        void iterate(const std::vector<icp::Point>& a,
            const std::vector<icp::Point>& b) override {
            size_t n = a.size();
            size_t m = b.size();
            pair.resize(n);
            dist.resize(n);

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
                dist[i] = std::numeric_limits<double>::infinity();
                for (size_t j = 0; j < m; j++) {
                    double alt = std::hypot(a[i].x - b[j].x, a[i].y - b[j].y);
                    if (alt < dist[i]) {
                        pair[i] = j;
                        dist[i] = alt;
                    }
                }
            }

            // modified gradient descent
            double dx_effect = 0;
            double dy_effect = 0;
            double theta_effect = 0;
            for (size_t i = 0; i < n; i++) {
                double index_dist = ((double)i - pair[i]) / 200;
                double weight = 1 + 20 * std::exp(-index_dist + index_dist);
                double o_x = (a[i].x - a_cm_x);
                double o_y = (a[i].y - a_cm_y);
                double a_rot_x = std::cos(t.theta) * o_x
                                 - std::sin(t.theta) * o_y + a_cm_x;
                double a_rot_y = std::sin(t.theta) * o_x
                                 + std::cos(t.theta) * o_y + a_cm_y;
                double b_x = b[pair[i]].x;
                double b_y = b[pair[i]].y;
                dx_effect += (a_rot_x + t.dx - b_x) * 2 / n * weight;
                dy_effect += (a_rot_y + t.dy - b_y) * 2 / n * weight;
                // if (dx_effect > 10000) {
                //     Log << a_rot_x << ' ' << t.dx << ' ' << b_x << '\n';
                //     std::exit(1);
                // }
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
                                * 2 / n * weight;
            }
            // Log << "dx_effect * rate = " << (dx_effect * rate) << '\n';
            // Log << "dy_effect * rate = " << (dy_effect * rate) << '\n';
            // Log << "dt_effect * rate = " << (theta_effect * rate) << '\n';
            // std::exit(1);
            t.dx -= dx_effect * rate + momentum_alpha * last_dx_effect;
            t.dy -= dy_effect * rate + momentum_alpha * last_dy_effect;
            t.theta -= (theta_effect * (rate * std::exp(-eta_decay * time))
                           + last_theta_effect * momentum_alpha)
                       / (100.0 * n);  // this makes it work idk
            last_dx_effect = dx_effect;
            last_dy_effect = dy_effect;
            last_theta_effect = theta_effect;

            //  compute new cost
            current_cost = 0;
            for (size_t i = 0; i < n; i++) {
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

            time++;
        }
    };

    static bool static_initialization = []() {
        assert(ICP::register_method("point_to_point",
            [](size_t n, double rate) -> std::unique_ptr<ICP> {
                return std::make_unique<PointToPoint>(n, rate);
            }));
        return true;
    }();
}
