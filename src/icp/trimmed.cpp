/*
 * @author Ethan Uppal
 * @copyright Copyright (C) 2024 Ethan Uppal. All rights reserved.
 */

#include <cassert>
#include <cstdlib>
#include "icp.h"
#include <Eigen/Core>
#include <Eigen/SVD>

/* #name Trimmed */

namespace icp {
    struct Trimmed final : public ICP {
        double overlap_rate;
        std::vector<icp::Vector> a_rot;

        Trimmed(double overlap_rate): ICP(), overlap_rate(overlap_rate) {}
        ~Trimmed() override {}

        void setup() override {
            if (a_rot.size() < a.size()) {
                a_rot.resize(a.size());
            }
        }

        void iterate() override {
            size_t n = a.size();
            const size_t m = b.size();

            for (size_t i = 0; i < n; i++) {
                a_rot[i] = transform.rotation * a[i];
            }

            /*
                #step
                Matching Step: match closest points

                Sources:
                https://arxiv.org/pdf/2206.06435.pdf
                https://web.archive.org/web/20220615080318/https://www.cs.technion.ac.il/~cs236329/tutorials/ICP.pdf
                https://en.wikipedia.org/wiki/Iterative_closest_point
                https://courses.cs.duke.edu/spring07/cps296.2/scribe_notes/lecture24.pdf
                -> use k-d tree
             */
            for (size_t i = 0; i < n; i++) {
                matches[i].point = i;
                matches[i].sq_dist = std::numeric_limits<double>::infinity();
                for (size_t j = 0; j < m; j++) {
                    // Point-to-point matching
                    double dist_ij = (b[j] - a_rot[i]).squaredNorm();

                    if (dist_ij < matches[i].sq_dist) {
                        matches[i].sq_dist = dist_ij;
                        matches[i].pair = j;
                    }
                }
            }

            /*
                #step
                Trimming Step

                Sources:
                https://ieeexplore.ieee.org/abstract/document/1047997
            */
            std::sort(matches.begin(), matches.end(),
                [](const auto& a, const auto& b) {
                    return a.sq_dist < b.sq_dist;
                });
            n = (size_t)(overlap_rate * n);

            /*
                #step
                Transformation Step: determine optimal transformation

                The translation vector is determined by the displacement between
                the centroids of both point clouds. The rotation matrix is
                calculated via singular value decomposition.

                Sources:
                https://courses.cs.duke.edu/spring07/cps296.2/scribe_notes/lecture24.pdf
             */
            transform.translation = b_cm - transform.rotation * a_cm;

            Matrix N{};
            for (size_t i = 0; i < n; i++) {
                N += a[matches[i].point] * b[matches[i].pair].transpose();
            }
            auto svd = N.jacobiSvd(Eigen::ComputeFullU | Eigen::ComputeFullV);
            const Matrix U = svd.matrixU();
            const Matrix V = svd.matrixV();
            transform.rotation = V * U.transpose();
        }
    };

    static bool static_initialization = []() {
        assert(ICP::register_method("trimmed",
            [](const ICP::Config& config) -> std::unique_ptr<ICP> {
                double overlap_rate = config.get<double>("overlap_rate", 1.0);
                assert(overlap_rate >= 0 && overlap_rate <= 1);
                return std::make_unique<Trimmed>(overlap_rate);
            }));
        return true;
    }();
}
