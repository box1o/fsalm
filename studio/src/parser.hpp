// #include <cstdlib>
// #include <cxxopts.hpp>
// #include <toolbox/base/base.hpp>
//
// cxxopts::ParseResult ParseCommandLine(int argc, char* argv[]) {
//     cxxopts::Options options("fslam", "SLAM system");
//     options.add_options()("v,video", "Path to video file, camera index, or stream URL",
//         cxxopts::value<std::string>())("c,camera", "Path to camera YAML config",
//         cxxopts::value<std::string>())("h,help", "Print usage");
//
//     auto args = options.parse(argc, argv);
//
//     if (args.count("help") || !args.count("video") || !args.count("camera")) {
//         ct::log::Info("{}", options.help());
//         std::abort();
//     }
//
//     return args;
//
// }
