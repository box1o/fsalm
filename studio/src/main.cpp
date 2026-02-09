#include <cxxopts.hpp>
#include <toolbox/base/base.hpp>
#include <toolbox/vision/vision.hpp>

#include <glad/glad.h>
#include <toolbox/gfx/gfx.hpp>

#include <opencv2/opencv.hpp>

#include "parser.hpp"



using namespace ct;

int main(int argc, char* argv[]) {
    log::Configure("toolbox", log::Level::Debug);
    log::Info("Starting toolbox...");

    auto window = Window({.title = "Toolbox", .width = 1280, .height = 720});

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        log::Critical("Failed to initialize GLAD");
        return -1;
    }

    while (!window.ShouldClose()) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        window.PollEvents();
        window.SwapBuffers();
    }
}




//     auto args = ParseCommandLine(argc, argv);
//
//     auto reader = Reader::Create({.path = args["video"].as<std::string>()}).value();
//     auto camera = Camera::FromYaml(args["camera"].as<std::string>()).value();
//
//     std::print("K =: {}", camera->intrinsics().K());
//     std::cout << "cvK =: \n{}" << camera->intrinsics().cvK();
//
//     FrontendInfo info{.camera = camera};
//     auto frontend = Frontend(info);
//
//
//
//
//     for (const auto& [image, ts] : *reader) {
//         cv::imshow("Frame", image);
//
//         auto pose = frontend.Estimate(image, ts);
//         if (pose) {
//             std::print("Estimated pose at time {}:\nRotation:\n{}\nTranslation:\n{}\n", ts,
//                 pose->rotation, pose->translation);
//         } else {
//             log::Error("Pose estimation failed at time {}: {}", ts, pose.error().Message());
//         }
//
//         // if (cv::waitKey(30) >= 0) {
//         //     log::Info("ESC key pressed. Exiting...");
//         //     break;
//         // }
//     }
// }




