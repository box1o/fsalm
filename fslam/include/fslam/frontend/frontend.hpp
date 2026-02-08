#pragma once
#include <opencv2/core/types.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/opencv.hpp>

#include "fslam/sensors/camera.hpp"
#include "fslam/types.hpp"

#include "fslam/core/result.hpp"

namespace fs {
struct FrontendInfo {
    ref<Camera> camera;
};

class Frontend {
public:
    Frontend(const FrontendInfo& info);
    ~Frontend();

    Frame DetectFeatures(const cv::Mat& gray, Timestamp ts);
    Matches Match(const Frame& curr, const Frame& prev);

    result<Pose> Estimate(const cv::Mat& image, Timestamp ts);

private:
    FrontendInfo mInfo;
    Frame mPrevFrame;

    // Compute descriptors
    cv::Mat gray;
    ref<cv::ORB> orb;

    // Matcher
    cv::BFMatcher matcher;
};

} // namespace fs
