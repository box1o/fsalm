#include <fslam/slam.hpp>
#include <opencv2/opencv.hpp>

using namespace fs;

int main(int /*argc*/, char* /*argv*/[]) {
    log::Configure("fslam", log::Level::Debug);

    auto reader = Reader::Create({
        .path = "/home/toor/Code/fslam/dataset/video.mp4",
        .camera = Camera::FromYaml("/home/toor/Code/fslam/dataset/camera.yaml")
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
