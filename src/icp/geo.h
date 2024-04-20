/*
 * @author Ethan Uppal
 * @copyright Copyright (C) 2024 Ethan Uppal. All rights reserved.
 */

#pragma once

#include <vector>
#include <cmath>
#include <Eigen/Core>

namespace icp {
    using Vector = Eigen::Vector2d;
    using Matrix = Eigen::Matrix2d;

    /** Rigid-body transformation. */
    struct RBTransform final {
        Vector translation;
        Matrix rotation;

    public:
        RBTransform(): translation() {
            rotation = Matrix::Identity();
        }
        RBTransform(Vector translation, Matrix rotation)
            : translation(translation), rotation(rotation) {}

        Vector apply_to(Vector v) const {
            return rotation * v + translation;
        }
    };

    Vector get_centroid(const std::vector<Vector>& points);
}
