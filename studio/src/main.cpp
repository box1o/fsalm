#include <fslam/slam.hpp>
#include <opencv2/opencv.hpp>
#include <cxxopts.hpp>

using namespace fs;

int main(int argc, char* argv[]) {
    log::Configure("fslam", log::Level::Debug);
    cxxopts::Options options("fslam", "SLAM system");
    options.add_options()
        ("v,video",  "Path to video file", cxxopts::value<std::string>())
        ("c,camera", "Path to camera YAML config", cxxopts::value<std::string>())
        ("h,help",   "Print usage");

    auto result = options.parse(argc, argv);



    // NOTE: Reader for the video 
    ref<Reader> reader = Reader::Create({
        .path = std::filesystem::path(result["video"].as<std::string>()),
    });



    for (const auto& [image, timestamp] : *reader) {
        // log::Info("Frame timestamp: {}", timestamp);
        cv::imshow("Frame", image);
        if (cv::waitKey(30) >= 0) {
            log::Info("ESC key pressed. Exiting...");
            break;
        }

    }

    return 0;
}
