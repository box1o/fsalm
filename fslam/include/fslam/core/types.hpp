#pragma once

#include <Eigen/Core>
#include <Eigen/Geometry>
#include <vector>
#include <cstdint>

#include <opencv2/core.hpp>

namespace fs {

using u8  = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

using i8  = std::int8_t;
using i16 = std::int16_t;
using i32 = std::int32_t;
using i64 = std::int64_t;

using f32 = float;
using f64 = double;

template<typename T>
using ref = std::shared_ptr<T>;

template<typename T>
using scope = std::unique_ptr<T>;

template<typename T>
using weak = std::weak_ptr<T>;

template<typename T, typename... Args>
constexpr ref<T> createRef(Args&&... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T, typename... Args>
constexpr scope<T> createScope(Args&&... args) {
    return std::make_unique<T>(std::forward<Args>(args)...);
}


// 

using Timestamp = double;
using FrameId = std::uint64_t;
using KeyframeId = std::uint64_t;
using LandmarkId = std::uint64_t;

using Vec2d = Eigen::Vector2d;
using Vec3d = Eigen::Vector3d;
using Vec4d = Eigen::Vector4d;
using Mat3d = Eigen::Matrix3d;
using Mat4d = Eigen::Matrix4d;
using SE3d = Eigen::Isometry3d;

using Keypoints = std::vector<cv::KeyPoint>;
using Descriptors = cv::Mat;

enum class TrackingState { 
    NotInitialized, 
    Initializing, 
    Tracking, 
    Lost 
};

struct Image {
    cv::Mat data;
    Timestamp timestamp{0.0};
    FrameId id{0};
};

} // namespace slam
