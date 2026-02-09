#pragma once

#include "toolbox/base/base.hpp"
#include "toolbox/vision/sensors/camera.hpp"
#include "toolbox/vision/types.hpp"

#include <opencv2/core/types.hpp>

#include <opencv2/features2d.hpp>
#include <opencv2/opencv.hpp>



namespace ct {

struct FrontendInfo {
    ref<ct::Camera> camera;
};

class Frontend {
public:
    explicit Frontend(const FrontendInfo& info);
    ~Frontend();

    [[nodiscard]] result<Pose> Estimate(const cv::Mat& image, Timestamp ts);

    [[nodiscard]] Frame DetectFeatures(const cv::Mat& gray, Timestamp ts);
    [[nodiscard]] Matches MatchFrames(const Frame& curr, const Frame& prev);


private:
    FrontendInfo mInfo;
    Frame mPrevFrame;

    cv::Mat mGray;
    ref<cv::ORB> mOrb;
    cv::BFMatcher mMatcher;
};

} // namespace fs
