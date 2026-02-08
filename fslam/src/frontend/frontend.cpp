#include "fslam/frontend/frontend.hpp"
#include "fslam/core/errors.hpp"
#include "fslam/core/logger.hpp"
#include "fslam/types.hpp"
#include <opencv2/core/types.hpp>
#include <vector>

namespace fs {

Frontend::Frontend(const FrontendInfo& info) : mInfo(info) {
    orb = cv::ORB::create();
    matcher = cv::BFMatcher(cv::NORM_HAMMING);
};

Frontend::~Frontend() {};

result<Pose> Frontend::Estimate(const cv::Mat& image, Timestamp ts) {
    if (image.empty()) {
        return err(ErrorCode::INVALID_ARGUMENT, "Frontend::Estimate: input image is empty");
    }
    if (image.type() != CV_8UC3) {
        return err(
            ErrorCode::INVALID_ARGUMENT, "Frontend::Estimate: input image must be of type CV_8UC3");
    }

    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    // 1.NOTE: Detect features in the image
    auto frame = DetectFeatures(gray, ts);

    // 2.NOTE: Match features to map points
    Matches matches;
    if (mPrevFrame.valid()) {
        matches = Match(frame, mPrevFrame);
        log::Info("Frontend::Estimate: detected {} features, matched {} features", frame.kps.size(),
            matches.size());



        // 3.NOTE: Estimate pose
        if (!mInfo.camera) {
            return err(ErrorCode::INVALID_ARGUMENT, "Frontend::Estimate: camera is not set");
        }

        std::vector<cv::Point2f> pts_curr, pts_prev;
        for (const auto& m : matches) {
            pts_curr.push_back(frame.kps[m.queryIdx].pt);
            pts_prev.push_back(mPrevFrame.kps[m.trainIdx].pt);
        }

        if (pts_curr.size() < 5) {
            log::Warn("Frontend::Estimate: not enough matches to estimate pose");
            mPrevFrame = std::move(frame);
            return err(ErrorCode::NOT_ENOUGH_FEATURES, "Frontend::Estimate: not enough matches");
        }



        auto [E, mask] = cv::findEssentialMat(pts_curr, pts_prev, mInfo.camera->intrinsics().matrix(), cv::RANSAC , 0.999, 1.0);
        if (E.empty()) {
            log::Warn("Frontend::Estimate: failed to find essential matrix");
            mPrevFrame = std::move(frame);
            return err(ErrorCode::UNKNOWN_ERROR, "Frontend::Estimate: failed to find essential matrix");
        }

        cv::Mat R, t;
        int inliers = cv::recoverPose(E, pts_curr, pts_prev, mInfo.camera->intrinsics().matrix(), R, t, mask);
        if (inliers < 5) {
            log::Warn("Frontend::Estimate: not enough inliers to recover pose");
            mPrevFrame = std::move(frame);
            return err(ErrorCode::NOT_ENOUGH_FEATURES, "Frontend::Estimate: not enough inliers");
        }


        Pose pose;
        pose.transform = SE3d(R, t);
        mPrevFrame = std::move(frame);
        return ok(pose);
    }


    mPrevFrame = std::move(frame);
    return err(ErrorCode::UNKNOWN_ERROR, "Frontend::Estimate: failed to estimate pose");
}

Frame Frontend::DetectFeatures(const cv::Mat& gray, Timestamp ts) {
    assert(!gray.empty());
    assert(image.type() == CV_8UC1);

    std::vector<cv::Point2f> corners;
    cv::goodFeaturesToTrack(gray, corners, 500, 0.01, 5);

    std::vector<cv::KeyPoint> kps;
    for (size_t i = 0; i < corners.size(); ++i) {
        kps.emplace_back(corners[i], 5.0f);
    }

    Frame f;
    f.kps = std::move(kps);
    f.timestamp = ts;
    orb->compute(gray, f.kps, f.des);
    return f;
}

Matches Frontend::Match(const Frame& curr, const Frame& prev) {
    std::vector<std::vector<cv::DMatch>> knn_matches;
    matcher.knnMatch(curr.des, mPrevFrame.des, knn_matches, 2);

    Matches good;
    good.reserve(knn_matches.size());
    const float ratio = 0.75f;

    for (auto& m : knn_matches) {
        if (m.size() < 2) continue;
        if (m[0].distance < ratio * m[1].distance) {
            good.push_back(m[0]);
        }
    }

    std::sort(good.begin(), good.end(),
        [](const cv::DMatch& a, const cv::DMatch& b) { return a.distance < b.distance; });
    if (good.size() > 200) good.resize(200);

    return good;
}

} // namespace fs
