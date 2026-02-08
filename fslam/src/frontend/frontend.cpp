#include "fslam/frontend/frontend.hpp"
#include "fslam/core/errors.hpp"
#include "fslam/core/logger.hpp"
#include "fslam/types.hpp"
#include <cassert>

namespace fs {

Frontend::Frontend(const FrontendInfo& info)
    : mInfo(info), mOrb(cv::ORB::create()), mMatcher(cv::NORM_HAMMING) {}

Frontend::~Frontend() = default;

result<Pose> Frontend::Estimate(const cv::Mat& image, Timestamp ts) {
    if (image.empty()) return err(ErrorCode::INVALID_ARGUMENT, "Input image is empty");

    if (image.type() != CV_8UC3)
        return err(ErrorCode::INVALID_ARGUMENT, "Input image must be CV_8UC3");

    cv::cvtColor(image, mGray, cv::COLOR_BGR2GRAY);

    Frame frame = DetectFeatures(mGray, ts);

    if (!frame.valid()) {
        mPrevFrame = std::move(frame);
        return err(ErrorCode::NOT_ENOUGH_FEATURES, "Failed to detect valid features");
    }

    if (!mPrevFrame.valid()) {
        mPrevFrame = std::move(frame);
        return err(ErrorCode::UNKNOWN_ERROR, "No previous frame yet");
    }

    Matches matches = MatchFrames(frame, mPrevFrame);
    log::Info("Detected {} features, matched {}", frame.kps.size(), matches.size());

    if (!mInfo.camera) return err(ErrorCode::INVALID_ARGUMENT, "Camera is not set");

    std::vector<cv::Point2f> ptsCurr, ptsPrev;
    ptsCurr.reserve(matches.size());
    ptsPrev.reserve(matches.size());

    for (const auto& m : matches) {
        ptsCurr.push_back(frame.kps[m.queryIdx].pt);
        ptsPrev.push_back(mPrevFrame.kps[m.trainIdx].pt);
    }

    if (ptsCurr.size() < 5) {
        log::Warn("Not enough matches to estimate pose: {}", ptsCurr.size());
        mPrevFrame = std::move(frame);
        return err(ErrorCode::NOT_ENOUGH_FEATURES, "Not enough matches");
    }

    // NOTE: Convert Eigen intrinsics to cv::Mat for OpenCV
    cv::Mat K = mInfo.camera->intrinsics().cvMatrix();

    cv::Mat mask;
    cv::Mat E = cv::findEssentialMat(ptsCurr, ptsPrev, K, cv::RANSAC, 0.999, 1.0, mask);

    if (E.empty()) {
        log::Warn("Failed to find essential matrix");
        mPrevFrame = std::move(frame);
        return err(ErrorCode::UNKNOWN_ERROR, "Essential matrix computation failed");
    }

    cv::Mat R, t;
    int inliers = cv::recoverPose(E, ptsCurr, ptsPrev, K, R, t, mask);

    if (inliers < 5) {
        log::Warn("Not enough inliers to recover pose: {}", inliers);
        mPrevFrame = std::move(frame);
        return err(ErrorCode::NOT_ENOUGH_FEATURES, "Not enough inliers");
    }

    // NOTE: Convert cv::Mat R,t to Eigen Isometry3d
    Pose pose;
    pose.transform = cvToSE3(R, t);
    mPrevFrame = std::move(frame);
    return ok(std::move(pose));
}

Frame Frontend::DetectFeatures(const cv::Mat& gray, Timestamp ts) {
    assert(!gray.empty());
    assert(gray.type() == CV_8UC1);

    std::vector<cv::Point2f> corners;
    cv::goodFeaturesToTrack(gray, corners, 500, 0.01, 5);

    std::vector<cv::KeyPoint> kps;
    kps.reserve(corners.size());
    for (const auto& corner : corners) kps.emplace_back(corner, 5.0f);

    Frame f;
    f.kps = std::move(kps);
    f.timestamp = ts;
    mOrb->compute(gray, f.kps, f.des);
    return f;
}

Matches Frontend::MatchFrames(const Frame& curr, const Frame& prev) {
    std::vector<std::vector<cv::DMatch>> knnMatches;
    mMatcher.knnMatch(curr.des, prev.des, knnMatches, 2);

    Matches good;
    good.reserve(knnMatches.size());
    constexpr float kRatioThreshold = 0.75f;

    for (const auto& m : knnMatches) {
        if (m.size() < 2) continue;
        if (m[0].distance < kRatioThreshold * m[1].distance) good.push_back(m[0]);
    }

    std::sort(good.begin(), good.end(),
        [](const cv::DMatch& a, const cv::DMatch& b) { return a.distance < b.distance; });

    if (good.size() > 200) good.resize(200);

    return good;
}

} // namespace fs
