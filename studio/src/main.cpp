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

#include <cxxopts.hpp>
#include <fslam/slam.hpp>
#include <opencv2/opencv.hpp>
#include <open3d/Open3D.h>

using namespace fs;

//NOTE: Convert Eigen::Isometry3d to Open3D 4x4 matrix
Eigen::Matrix4d toMatrix4d(const SE3d& pose) {
    return pose.matrix();
}

int main(int argc, char* argv[]) {
    log::Configure("fslam", log::Level::Debug);

    cxxopts::Options options("fslam", "SLAM system");
    options.add_options()
        ("v,video", "Path to video file", cxxopts::value<std::string>())
        ("c,camera", "Path to camera YAML config", cxxopts::value<std::string>())
        ("h,help", "Print usage");

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

    Frontend frontend(FrontendInfo{.camera = cameraResult.value()});

    //NOTE: Open3D visualization setup
    auto vis = std::make_shared<open3d::visualization::Visualizer>();
    vis->CreateVisualizerWindow("FSLAM - Camera Trajectory", 1280, 720);

    auto renderOpt = vis->GetRenderOption();
    renderOpt.background_color_ = Eigen::Vector3d(0.1, 0.1, 0.1);
    renderOpt.point_size_ = 3.0;

    //NOTE: World origin frame
    auto originFrame = open3d::geometry::TriangleMesh::CreateCoordinateFrame(0.3, Eigen::Vector3d::Zero());
    vis->AddGeometry(originFrame);

    //NOTE: Trajectory line
    auto trajectory = std::make_shared<open3d::geometry::LineSet>();
    vis->AddGeometry(trajectory);

    SE3d globalPose = SE3d::Identity();
    std::vector<Eigen::Vector3d> positions;
    std::vector<std::shared_ptr<open3d::geometry::TriangleMesh>> cameraFrames;

    for (const auto& [image, ts] : *reader) {
        auto poseResult = frontend.Estimate(image, ts);

        if (poseResult.has_value()) {
            //NOTE: Accumulate global pose from relative transforms
            globalPose = globalPose * poseResult.value().transform;

            Eigen::Vector3d pos = globalPose.translation();
            positions.push_back(pos);

            //NOTE: Draw camera coordinate frame at current pose
            auto camFrame = open3d::geometry::TriangleMesh::CreateCoordinateFrame(0.08);
            camFrame->Transform(toMatrix4d(globalPose));
            vis->AddGeometry(camFrame);
            cameraFrames.push_back(camFrame);

            //NOTE: Update trajectory line connecting all poses
            if (positions.size() >= 2) {
                trajectory->points_ = positions;
                trajectory->lines_.clear();
                trajectory->colors_.clear();
                for (size_t i = 0; i < positions.size() - 1; ++i) {
                    trajectory->lines_.push_back({static_cast<int>(i), static_cast<int>(i + 1)});
                    trajectory->colors_.push_back({0.0, 1.0, 0.0});
                }
                vis->UpdateGeometry(trajectory);
            }

            //NOTE: Center view on first few frames
            if (positions.size() == 1)
                vis->ResetViewPoint(true);
        }

        vis->PollEvents();
        vis->UpdateRender();

        cv::imshow("Frame", image);
        if (cv::waitKey(1) >= 0) {
            log::Info("Key pressed. Exiting...");
            break;
        }
    }

    log::Info("Total poses estimated: {}", positions.size());

    //NOTE: Keep window open after processing
    while (vis->PollEvents()) {
        vis->UpdateRender();
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    vis->DestroyVisualizerWindow();
    cv::destroyAllWindows();
    return 0;
}
