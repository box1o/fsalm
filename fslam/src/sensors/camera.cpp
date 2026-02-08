#include "fslam/sensors/camera.hpp"
#include "fslam/core/logger.hpp"
#include <yaml-cpp/yaml.h>

namespace fs {

Camera::Camera(
    CameraType type, const CameraIntrinsics& intrinsics, const DistortionCoeffs& distortion)
    : mType(type), mIntrinsics(intrinsics), mDistortion(distortion) {}

result<ref<Camera>> Camera::FromYaml(const std::filesystem::path& path) {
    if (!std::filesystem::exists(path))
        return err(
            ErrorCode::INVALID_ARGUMENT, "Camera YAML file does not exist: " + path.string());

    try {
        YAML::Node config = YAML::LoadFile(path.string());
        auto cam = config["camera"];

        if (!cam) return err(ErrorCode::INVALID_ARGUMENT, "Missing 'camera' key in YAML");

        std::string type = cam["type"].as<std::string>();

        auto intr = cam["intrinsics"];
        CameraIntrinsics intrinsics{.fx = intr["fx"].as<double>(),
            .fy = intr["fy"].as<double>(),
            .cx = intr["cx"].as<double>(),
            .cy = intr["cy"].as<double>(),
            .width = cam["resolution"]["width"].as<int>(),
            .height = cam["resolution"]["height"].as<int>()};

        auto dist = cam["distortion"];
        DistortionCoeffs distortion{.k1 = dist["k1"].as<double>(),
            .k2 = dist["k2"].as<double>(),
            .k3 = dist["k3"].as<double>(),
            .p1 = dist["p1"].as<double>(),
            .p2 = dist["p2"].as<double>()};

        CameraType camType;
        if (type == "monocular")
            camType = CameraType::Monocular;
        else if (type == "stereo")
            camType = CameraType::Stereo;
        else if (type == "rgbd")
            camType = CameraType::RGBD;
        else
            return err(ErrorCode::INVALID_ARGUMENT, "Unknown camera type: " + type);

        return ok(createRef<Camera>(camType, intrinsics, distortion));
    } catch (const YAML::Exception& e) {
        return err(ErrorCode::INVALID_ARGUMENT,
            "Failed to parse camera YAML '" + path.string() + "': " + e.what());
    }
}

} // namespace fs
