#include <cxxopts.hpp>
#include <fslam/slam.hpp>
#include <opencv2/opencv.hpp>

using namespace fs;

int main(int argc, char* argv[]) {
    log::Configure("fslam", log::Level::Debug);
    cxxopts::Options options("fslam", "SLAM system");
    options.add_options()
        ("v,video", "Path to video file", cxxopts::value<std::string>())
        ( "c,camera", "Path to camera YAML config", cxxopts::value<std::string>())
        ( "h,help", "Print usage");

    auto result = options.parse(argc, argv);
    auto vPath = result["video"].as<std::string>();
    auto cPath = result["camera"].as<std::string>();



    // NOTE: Reader for the video
     auto reader = Reader::Create({
        .path = vPath
    });

    FrontendInfo info{
        .camera = Camera::FromYaml(cPath)
    };

    auto frontend = Frontend(info);

    for (const auto& [image, ts] : *reader) {
        if (auto pose = frontend.Estimate(image, ts)) {
        }

        cv::imshow("Frame", image);
        if (cv::waitKey(30) >= 0) {
            log::Info("ESC key pressed. Exiting...");
            break;
        }
    }

    return 0;
}
