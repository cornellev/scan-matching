// Copyright (C) 2024 Ethan Uppal. All rights reserved.

#include <cassert>
#include <cstdlib>
#include "icp.h"
#include "util/logger.h"
#include <Eigen/SVD>

namespace icp {
    struct Vanilla final : public ICP {
        Vanilla(double rate): ICP(rate) {}
        ~Vanilla() override {}

        void setup(const std::vector<icp::Point>& a,
            const std::vector<icp::Point>& b) override {
            // ensure that the `pair` and `dist` arrays are large enough to
            // handle the input point clouds.
            size_t n = a.size();
            size_t m = b.size();
            pair.reserve(n);
            dist.reserve(n);

            /* Align center of mass.

               Sources:
               https://arxiv.org/pdf/2206.06435.pdf
               https://web.archive.org/web/20220615080318/https://www.cs.technion.ac.il/~cs236329/tutorials/ICP.pdf
             */
            Point a_cm = get_center_of_mass(a);
            Point b_cm = get_center_of_mass(b);
            t.cm = a_cm;
            t.dx = a_cm.x - a_cm.x;
            t.dy = b_cm.y - a_cm.y;
        }

        void iterate(const std::vector<icp::Point>& a,
            const std::vector<icp::Point>& b) override {
            /* Matching Step: match closest points.

               Sources:
               https://arxiv.org/pdf/2206.06435.pdf
               https://web.archive.org/web/20220615080318/https://www.cs.technion.ac.il/~cs236329/tutorials/ICP.pdf
               https://en.wikipedia.org/wiki/Iterative_closest_point
               https://courses.cs.duke.edu/spring07/cps296.2/scribe_notes/lecture24.pdf
               -> use k-d tree
             */
            for (size_t i = 0; i < n; i++) {
                dist[i] = std::numeric_limits<double>::infinity();
                for (size_t j = 0; j < m; j++) {
                    // Point-to-point matching
                    double dist_ij = std::hypot(b[i].x - a[i].x,
                        b[i].y - a[i].y);

                    if (dist_ij < dist[i]) {
                        dist[i] = dist_ij;
                        pair[i] = j;
                    }
                }
            }

            /* Transformation Step: */
        }
    };

    static bool static_initialization = []() {
        assert(ICP::register_method("vanilla",
            [](size_t n __unused, double rate) -> std::unique_ptr<ICP> {
                return std::make_unique<Vanilla>(rate);
            }));
        return true;
    }();
}
