/*
 * @author Ethan Uppal
 * @copyright Copyright (C) 2024 Ethan Uppal. All rights reserved.
 */

#include <cassert>
#include <cstdlib>
#include "icp.h"
#include "util/logger.h"
#include <Eigen/Core>
#include <Eigen/SVD>

namespace icp {
    struct Vanilla final : public ICP {
        std::vector<Vector> rotated_a;

        Vanilla(): ICP() {}
        ~Vanilla() override {}

        void iterate() override {
            const size_t n = a.size();
            const size_t m = b.size();

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
                    double dist_ij = (b[j] - a[i]).norm();

                    if (dist_ij < dist[i]) {
                        dist[i] = dist_ij;
                        pair[i] = j;
                    }
                }
            }

            /* Transformation Step: determine optimal transformation.

               The translation vector is determined by the displacement between
               the centroids of both point clouds. The rotation matrix is
               calculated via singular value decomposition.

               Sources:
               https://courses.cs.duke.edu/spring07/cps296.2/scribe_notes/lecture24.pdf
             */
            transform.translation = b_cm - transform.rotation * a_cm;

            Matrix N{};
            for (size_t i = 0; i < n; i++) {
                N += a[i] * b[i].transpose();
            }
            auto svd = N.jacobiSvd(Eigen::ComputeFullU | Eigen::ComputeFullV);
            assert(svd.computeU());
            const Matrix U = svd.matrixU();
            const Matrix V = svd.matrixV();
            transform.rotation = V * U.transpose();
        }
    };

    static bool static_initialization = []() {
        assert(ICP::register_method("vanilla", []() -> std::unique_ptr<ICP> {
            return std::make_unique<Vanilla>();
        }));
        return true;
    }();
}
