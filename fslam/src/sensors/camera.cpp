#include "fslam/sensors/camera.hpp"
#include "fslam/core/types.hpp"
#include "fslam/core/logger.hpp"
#include <cstdlib>

#include <yaml-cpp/yaml.h>

namespace fs {

Camera::Camera(
    CameraType type, const CameraIntrinsics& intrinsics, const DistortionCoeffs& distortion)
: mType(type), mIntrinsics(intrinsics), mDistortion(distortion) {}

Vec3d Camera::Unproject(const Vec2d& px) const {}

Vec2d Camera::Project(const Vec3d& pt) const {}

cv::Mat Camera::Undistort(const cv::Mat& image) const {}

ref<Camera> Camera::FromYaml(const std::filesystem::path& path) {

    try {
        if (!std::filesystem::exists(path)) {
            throw std::runtime_error("Camera YAML file does not exist: " + path.string());
        }

        YAML::Node config = YAML::LoadFile(path.string());
        auto cam = config["camera"];
        std::string type = cam["type"].as<std::string>();

        auto intr = cam["intrinsics"];
        double fx = intr["fx"].as<double>();
        double fy = intr["fy"].as<double>();
        double cx = intr["cx"].as<double>();
        double cy = intr["cy"].as<double>();

        auto res = cam["resolution"];
        int width  = res["width"].as<int>();
        int height = res["height"].as<int>();

        auto dist = cam["distortion"];
        double k1 = dist["k1"].as<double>();
        double k2 = dist["k2"].as<double>();
        double p1 = dist["p1"].as<double>();
        double p2 = dist["p2"].as<double>();
        double k3 = dist["k3"].as<double>();

        CameraIntrinsics intrinsics {
            .fx = fx,
            .fy = fy,
            .cx = cx,
            .cy = cy,
            .width = width,
            .height = height
        };

        CameraType camType;
        if (type == "monocular") {
            camType = CameraType::Monocular;
        } else if (type == "stereo") {
            camType = CameraType::Stereo;
        } else if (type == "rgbd") {
            camType = CameraType::RGBD;
        } else {
            log::Critical("Unknown camera type: {}", type);
            std::abort();
        }

        DistortionCoeffs distortion {
            .k1 = k1,
            .k2 = k2,
            .k3 = k3,
            .p1 = p1,
            .p2 = p2
        };

        return createRef<Camera>(camType, intrinsics, distortion);
    } catch (const YAML::Exception& e) {
        log::Critical("Failed to load camera YAML '{}': {}", path.string(), e.what());
        throw; 
    }

    if (!std::filesystem::exists(path)) {
        log::Critical("Camera YAML file does not exist: {}", path.string());
        std::abort();
    }



}

} // namespace fs
