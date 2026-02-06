#include "fslam/pipeline/pipeline.hpp"
#include "fslam/sensors/camera.hpp"
#include <fslam/slam.hpp>
#include <opencv2/opencv.hpp>


using namespace fs;

int main(int /*argc*/, char* /*argv*/[]) {
    log::Configure("fslam", log::Level::Debug);


    auto pipeline = fs::Pipeline::Create()
        .WithCamera(fs::Camera::FromYaml("/home/toor/Code/fsalm/dataset/camera.yaml"))
        // .WithFrontend(
        //     Frontend::Create()
        //         .FeatureExtractor<OrbExtractor>(2000)
        //         .Tracker<OpticalFlowTracker>()
        //         .MotionModel<ConstantVelocity>()
        // )
        // .WithBackend(
        //     Backend::Create()
        //         .Optimizer<G2oOptimizer>()
        //         .BundleAdjustment<LocalBA>(windowSize: 10)
        //         .RunAsync(true)
        // )
        // .WithLoopClosure(
        //     LoopClosure::Create()
        //         .Detector<DBoWDetector>("vocabulary.dbow")
        //         .PoseGraph<PoseGraphOptimizer>()
        // )
        .Build();



    // if (!pipeline) {
    //     return 1;
    // }
    //
    auto reader = fs::Reader::Create({.path = "/home/toor/Code/fsalm/dataset/video.mp4"});

    for (const auto& [image, timestamp] : *reader) {
        // log::Info("Frame timestamp: {}", timestamp);
        cv::imshow("Frame", image);
        if (cv::waitKey(30) >= 0) {
            log::Info("ESC key pressed. Exiting...");
            break;
        }


        // auto result = pipeline.process(image, timestamp);
        // if (result.tracking == TrackingState::OK) {
        //     auto pose = result.pose();
        //     auto points = result.visibleLandmarks();
        // }


    }


    // pipeline.SaveTrajectory("trajectory.txt");
    // pipeline.SaveMap("map.ply");
    return 0;
}
