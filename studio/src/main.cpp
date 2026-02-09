// #include <cxxopts.hpp>
// #include <fslam/slam.hpp>
// #include <opencv2/core/mat.hpp>
// #include <opencv2/opencv.hpp>
// #include <open3d/Open3D.h>
//
// using namespace fs;
//
// int main(int argc, char* argv[]) {
//     log::Configure("fslam", log::Level::Debug);
//     cxxopts::Options options("fslam", "SLAM system");
//     options.add_options()
//         ("v,video", "Path to video file", cxxopts::value<std::string>())
//         ( "c,camera", "Path to camera YAML config", cxxopts::value<std::string>())
//         ( "h,help", "Print usage");
//
//     auto result = options.parse(argc, argv);
//     auto vPath = result["video"].as<std::string>();
//     auto cPath = result["camera"].as<std::string>();
//
//
//
//     // NOTE: Reader for the video
//      auto reader = Reader::Create({
//         .path = vPath
//     }).value();
//
//     FrontendInfo info{
//         .camera = Camera::FromYaml(cPath).value()
//     };
//
//     auto frontend = Frontend(info);
//
//     for (const auto& [image, ts] : *reader) {
//         cv::Mat gray;
//         cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
//
//         auto frame = frontend.DetectFeatures(gray, ts);
//         for (const auto& kp : frame.kps) {
//             cv::circle(image, kp.pt, 3, cv::Scalar(0, 255, 0), -1);
//         }
//
//
//         if (auto pose = frontend.Estimate(image, ts)) {
//         }
//
//         cv::imshow("Frame", image);
//         if (cv::waitKey(30) >= 0) {
//             log::Info("ESC key pressed. Exiting...");
//             break;
//         }
//     }
//
//     return 0;
// }

// #include <cxxopts.hpp>
// #include <fslam/slam.hpp>
// #include <opencv2/opencv.hpp>
// #include <open3d/Open3D.h>
//
// using namespace fs;
//
// //NOTE: Convert Eigen::Isometry3d to Open3D 4x4 matrix
// Eigen::Matrix4d toMatrix4d(const SE3d& pose) {
//     return pose.matrix();
// }
//
// int main(int argc, char* argv[]) {
//     log::Configure("fslam", log::Level::Debug);
//
//     cxxopts::Options options("fslam", "SLAM system");
//     options.add_options()
//         ("v,video", "Path to video file", cxxopts::value<std::string>())
//         ("c,camera", "Path to camera YAML config", cxxopts::value<std::string>())
//         ("h,help", "Print usage");
//
//     auto args = options.parse(argc, argv);
//
//     if (args.count("help") || !args.count("video") || !args.count("camera")) {
//         log::Info("{}", options.help());
//         return 0;
//     }
//
//     auto vPath = args["video"].as<std::string>();
//     auto cPath = args["camera"].as<std::string>();
//
//     auto readerResult = Reader::Create({.path = vPath});
//     if (!readerResult.has_value()) {
//         readerResult.error().Log();
//         return 1;
//     }
//     auto reader = std::move(readerResult.value());
//
//     auto cameraResult = Camera::FromYaml(cPath);
//     if (!cameraResult.has_value()) {
//         cameraResult.error().Log();
//         return 1;
//     }
//
//     Frontend frontend(FrontendInfo{.camera = cameraResult.value()});
//
//     //NOTE: Open3D visualization setup
//     auto vis = std::make_shared<open3d::visualization::Visualizer>();
//     vis->CreateVisualizerWindow("FSLAM - Camera Trajectory", 1280, 720);
//
//     auto renderOpt = vis->GetRenderOption();
//     renderOpt.background_color_ = Eigen::Vector3d(0.1, 0.1, 0.1);
//     renderOpt.point_size_ = 3.0;
//
//     //NOTE: World origin frame
//     auto originFrame = open3d::geometry::TriangleMesh::CreateCoordinateFrame(0.3,
//     Eigen::Vector3d::Zero()); vis->AddGeometry(originFrame);
//
//     //NOTE: Trajectory line
//     auto trajectory = std::make_shared<open3d::geometry::LineSet>();
//     vis->AddGeometry(trajectory);
//
//     SE3d globalPose = SE3d::Identity();
//     std::vector<Eigen::Vector3d> positions;
//     std::vector<std::shared_ptr<open3d::geometry::TriangleMesh>> cameraFrames;
//
//     for (const auto& [image, ts] : *reader) {
//         auto poseResult = frontend.Estimate(image, ts);
//
//         if (poseResult.has_value()) {
//             //NOTE: Accumulate global pose from relative transforms
//             globalPose = globalPose * poseResult.value().transform;
//
//             Eigen::Vector3d pos = globalPose.translation();
//             positions.push_back(pos);
//
//             //NOTE: Draw camera coordinate frame at current pose
//             auto camFrame = open3d::geometry::TriangleMesh::CreateCoordinateFrame(0.08);
//             camFrame->Transform(toMatrix4d(globalPose));
//             vis->AddGeometry(camFrame);
//             cameraFrames.push_back(camFrame);
//
//             //NOTE: Update trajectory line connecting all poses
//             if (positions.size() >= 2) {
//                 trajectory->points_ = positions;
//                 trajectory->lines_.clear();
//                 trajectory->colors_.clear();
//                 for (size_t i = 0; i < positions.size() - 1; ++i) {
//                     trajectory->lines_.push_back({static_cast<int>(i), static_cast<int>(i + 1)});
//                     trajectory->colors_.push_back({0.0, 1.0, 0.0});
//                 }
//                 vis->UpdateGeometry(trajectory);
//             }
//
//             //NOTE: Center view on first few frames
//             if (positions.size() == 1)
//                 vis->ResetViewPoint(true);
//         }
//
//         vis->PollEvents();
//         vis->UpdateRender();
//
//
//         // cv::imshow("Frame", image);
//     //     if (cv::waitKey(1) >= 0) {
//     //         log::Info("Key pressed. Exiting...");
//     //         break;
//     //     }
//     }
//
//     log::Info("Total poses estimated: {}", positions.size());
//
//     //NOTE: Keep window open after processing
//     while (vis->PollEvents()) {
//         vis->UpdateRender();
//         std::this_thread::sleep_for(std::chrono::milliseconds(16));
//     }
//
//     vis->DestroyVisualizerWindow();
//     cv::destroyAllWindows();
//     return 0;
// }

#include <chrono>
#include <cxxopts.hpp>
#include <fslam/slam.hpp>
#include <open3d/Open3D.h>
#include <opencv2/opencv.hpp>
#include <thread>

using namespace fs;

// NOTE: Build a camera frustum as a LineSet from intrinsics and pose
std::shared_ptr<open3d::geometry::LineSet> CreateFrustum(
    const SE3d& pose, const CameraIntrinsics& intr, double scale, const Eigen::Vector3d& color) {

    double w = static_cast<double>(intr.width);
    double h = static_cast<double>(intr.height);
    double fx = intr.fx;
    double fy = intr.fy;
    double cx = intr.cx;
    double cy = intr.cy;

    // NOTE: 4 corners of image plane unprojected to z=scale
    Eigen::Vector3d tl((-cx) / fx * scale, (-cy) / fy * scale, scale);
    Eigen::Vector3d tr((w - cx) / fx * scale, (-cy) / fy * scale, scale);
    Eigen::Vector3d bl((-cx) / fx * scale, (h - cy) / fy * scale, scale);
    Eigen::Vector3d br((w - cx) / fx * scale, (h - cy) / fy * scale, scale);
    Eigen::Vector3d origin(0.0, 0.0, 0.0);

    // NOTE: Transform to world frame
    Eigen::Matrix4d T = pose.matrix();
    auto xform = [&](const Eigen::Vector3d& p) -> Eigen::Vector3d {
        Eigen::Vector4d ph(p.x(), p.y(), p.z(), 1.0);
        Eigen::Vector4d w4 = T * ph;
        return w4.head<3>();
    };

    auto frustum = std::make_shared<open3d::geometry::LineSet>();

    frustum->points_ = {
        xform(origin), // 0: camera center
        xform(tl),     // 1: top-left
        xform(tr),     // 2: top-right
        xform(br),     // 3: bottom-right
        xform(bl),     // 4: bottom-left
    };

    // NOTE: 8 lines: 4 edges from center to corners + 4 edges of image rectangle
    frustum->lines_ = {
        {0, 1},
        {0, 2},
        {0, 3},
        {0, 4},
        {1, 2},
        {2, 3},
        {3, 4},
        {4, 1},
    };

    frustum->colors_.resize(8, color);

    return frustum;
}

// NOTE: Build a small axes indicator at the camera pose
std::shared_ptr<open3d::geometry::LineSet> CreateAxes(const SE3d& pose, double length) {
    Eigen::Matrix4d T = pose.matrix();

    auto xform = [&](const Eigen::Vector3d& p) -> Eigen::Vector3d {
        Eigen::Vector4d ph(p.x(), p.y(), p.z(), 1.0);
        return (T * ph).head<3>();
    };

    auto axes = std::make_shared<open3d::geometry::LineSet>();

    Eigen::Vector3d o(0, 0, 0);
    axes->points_ = {
        xform(o),
        xform(Eigen::Vector3d(length, 0, 0)),
        xform(Eigen::Vector3d(0, length, 0)),
        xform(Eigen::Vector3d(0, 0, length)),
    };

    axes->lines_ = {{0, 1}, {0, 2}, {0, 3}};
    axes->colors_ = {
        {1.0, 0.0, 0.0}, // X red
        {0.0, 1.0, 0.0}, // Y green
        {0.0, 0.0, 1.0}, // Z blue
    };

    return axes;
}

int main(int argc, char* argv[]) {
    log::Configure("fslam", log::Level::Debug);

    cxxopts::Options options("fslam", "SLAM system");
    options.add_options()("v,video", "Path to video file, camera index, or stream URL",
        cxxopts::value<std::string>())("c,camera", "Path to camera YAML config",
        cxxopts::value<std::string>())("h,help", "Print usage");

    auto args = options.parse(argc, argv);

    if (args.count("help") || !args.count("video") || !args.count("camera")) {
        log::Info("{}", options.help());
        return 0;
    }

    auto vPath = args["video"].as<std::string>();
    auto cPath = args["camera"].as<std::string>();

    auto readerResult = Reader::Create({.path = vPath});
    if (!readerResult.has_value()) {
        readerResult.error().Log();
        return 1;
    }
    auto reader = std::move(readerResult.value());

    auto cameraResult = Camera::FromYaml(cPath);
    if (!cameraResult.has_value()) {
        cameraResult.error().Log();
        return 1;
    }
    auto cam = cameraResult.value();

    Frontend frontend(FrontendInfo{.camera = cam});

    auto vis = std::make_shared<open3d::visualization::Visualizer>();
    vis->CreateVisualizerWindow("FSLAM - Camera Trajectory", 1280, 720);

    auto& renderOpt = vis->GetRenderOption();
    renderOpt.background_color_ = Eigen::Vector3d(0.05, 0.05, 0.1);
    renderOpt.line_width_ = 2.0;

    // NOTE: World origin
    auto originAxes =
        open3d::geometry::TriangleMesh::CreateCoordinateFrame(0.5, Eigen::Vector3d::Zero());
    vis->AddGeometry(originAxes);

    // NOTE: Trajectory connecting camera centers
    auto trajectory = std::make_shared<open3d::geometry::LineSet>();
    vis->AddGeometry(trajectory);

    SE3d globalPose = SE3d::Identity();
    std::vector<Eigen::Vector3d> positions;

    // NOTE: Frustum visualization scale
    constexpr double kFrustumScale = 0.15;
    constexpr double kAxesLength = 0.06;
    constexpr int kMaxFrustumsDrawn = 500;
    int frameCount = 0;

    // NOTE: Color gradient from cyan to magenta along trajectory
    auto frustumColor = [](int idx, int total) -> Eigen::Vector3d {
        double t = (total > 1) ? static_cast<double>(idx) / (total - 1) : 0.0;
        return Eigen::Vector3d(t, 1.0 - 0.5 * t, 1.0 - t);
    };

    cv::Mat gray;
    for (const auto& [image, ts] : *reader) {
        auto poseResult = frontend.Estimate(image, ts);

        if (poseResult.has_value()) {
            globalPose = globalPose * poseResult.value().transform;
            Eigen::Vector3d pos = globalPose.translation();
            positions.push_back(pos);
            frameCount++;

            // NOTE: Draw frustum every few frames to avoid flooding the scene
            bool drawFrustum =
                (frameCount <= kMaxFrustumsDrawn) && (frameCount <= 20 || frameCount % 5 == 0);

            if (drawFrustum) {
                auto color = frustumColor(frameCount, kMaxFrustumsDrawn);
                auto frustum = CreateFrustum(globalPose, cam->intrinsics(), kFrustumScale, color);
                vis->AddGeometry(frustum);

                auto axes = CreateAxes(globalPose, kAxesLength);
                vis->AddGeometry(axes);
            }

            // NOTE: Update trajectory line
            if (positions.size() >= 2) {
                trajectory->points_ = positions;
                trajectory->lines_.clear();
                trajectory->colors_.clear();
                for (size_t i = 0; i < positions.size() - 1; ++i) {
                    trajectory->lines_.push_back({static_cast<int>(i), static_cast<int>(i + 1)});
                    trajectory->colors_.push_back({0.0, 1.0, 0.3});
                }
                vis->UpdateGeometry(trajectory);
            }

            if (positions.size() == 1) vis->ResetViewPoint(true);
        }

        vis->PollEvents();
        vis->UpdateRender();



        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
        auto feat = frontend.DetectFeatures(gray, ts);
        for (const auto& kp : feat.kps) {
            cv::circle(image, kp.pt, 3, cv::Scalar(0, 255, 0), -1);
        }
        cv::imshow("Frame", image);
        // if (cv::waitKey(1) == 27) {
    //         log::Info("Key pressed. Exiting...");
    //         break;
    //     }
    }

    log::Info("Total poses estimated: {}", positions.size());
    log::Info("Frustums drawn: {}", frameCount);

    while (vis->PollEvents()) {
        vis->UpdateRender();
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    vis->DestroyVisualizerWindow();
    cv::destroyAllWindows();
    return 0;
}
