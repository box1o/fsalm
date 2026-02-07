#pragma once

#include "fslam/core/types.hpp"
#include <filesystem>

namespace fs {

struct CameraIntrinsics {
    double fx{0}, fy{0};
    double cx{0}, cy{0};
    int width{0}, height{0};
    //
    // Mat3d matrix() const {
    //     Mat3d K = Mat3d::Identity();
    //     K(0, 0) = fx;
    //     K(1, 1) = fy;
    //     K(0, 2) = cx;
    //     K(1, 2) = cy;
    //     return K;
    // }
};

struct DistortionCoeffs {
    double k1{0}, k2{0}, k3{0};
    double p1{0}, p2{0};

    // cv::Mat toOpenCV() const { return (cv::Mat_<double>(5, 1) << k1, k2, p1, p2, k3); }
};

enum class CameraType { Monocular, Stereo, RGBD };

class Camera {
public:
    Camera(CameraType type, const CameraIntrinsics& intrinsics, const DistortionCoeffs& distortion);

    CameraType type() const { return mType; }
    const CameraIntrinsics& intrinsics() const { return mIntrinsics; }
    const DistortionCoeffs& distortion() const { return mDistortion; }

    // Vec3d Unproject(const Vec2d& px) const;
    // Vec2d Project(const Vec3d& pt) const;
    // cv::Mat Undistort(const cv::Mat& image) const;

    static ref<Camera> FromYaml(const std::filesystem::path& path);

private:
    CameraType mType;
    CameraIntrinsics mIntrinsics;
    DistortionCoeffs mDistortion;
};

} // namespace fs
