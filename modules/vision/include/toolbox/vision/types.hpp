#pragma once
#include "toolbox/math/math.hpp"
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/core/types.hpp>

namespace ct {

using Timestamp = double;

struct Frame {
    Timestamp timestamp{0.0};
    std::vector<cv::KeyPoint> kps;
    cv::Mat des;

    bool valid() const {
        return !des.empty() && !kps.empty() && des.rows == static_cast<int>(kps.size());
    }
};

using Match = cv::DMatch;
using Matches = std::vector<Match>;

struct Pose {
    mat3d rotation{mat3d::identity()};
    vec3d translation{0.0, 0.0, 0.0};
};

using Poses = std::vector<Pose>;

} // namespace ct
