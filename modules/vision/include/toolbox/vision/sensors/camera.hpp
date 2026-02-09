#pragma once
#include "toolbox/base/base.hpp"
#include "toolbox/math/math.hpp"
#include <filesystem>

#include <opencv2/opencv.hpp>

namespace ct {

struct CameraIntrinsics {
    double fx{0}, fy{0};
    double cx{0}, cy{0};
    int width{0}, height{0};

    inline mat3d K() const { return mat3d(layout::rowm, fx, 0.0, cx, 0.0, fy, cy, 0.0, 0.0, 1.0); }

    cv::Mat cvK() const {
        cv::Mat K = cv::Mat::zeros(3, 3, CV_64F);
        K.at<double>(0, 0) = fx;
        K.at<double>(1, 1) = fy;
        K.at<double>(0, 2) = cx;
        K.at<double>(1, 2) = cy;
        K.at<double>(2, 2) = 1.0;
        return K;
    }
};

struct DistortionCoeffs {
    double k1{0}, k2{0}, k3{0};
    double p1{0}, p2{0};

    cv::Mat cvDistortion() const {
        cv::Mat distCoeffs = cv::Mat::zeros(5, 1, CV_64F);
        distCoeffs.at<double>(0, 0) = k1;
        distCoeffs.at<double>(1, 0) = k2;
        distCoeffs.at<double>(2, 0) = p1;
        distCoeffs.at<double>(3, 0) = p2;
        distCoeffs.at<double>(4, 0) = k3;
        return distCoeffs;
    }
};

enum class CameraType { Monocular, Stereo, RGBD };

class Camera {
public:
    Camera(CameraType type, const CameraIntrinsics& intrinsics, const DistortionCoeffs& distortion);

    [[nodiscard]] CameraType type() const { return mType; }
    [[nodiscard]] const CameraIntrinsics& intrinsics() const { return mIntrinsics; }
    [[nodiscard]] const DistortionCoeffs& distortion() const { return mDistortion; }

    [[nodiscard]] static result<ref<Camera>> FromYaml(const std::filesystem::path& path);

private:
    CameraType mType;
    CameraIntrinsics mIntrinsics;
    DistortionCoeffs mDistortion;
};

} // namespace ct
