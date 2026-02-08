#include "fslam/frontend/detector.hpp"
#include <opencv2/opencv.hpp>

namespace fs {

ORBDetector::ORBDetector() {
    orb = cv::ORB::create(/*dont care about ags because i will detect features otherways , need this for descriptors*/);

}
ORBDetector::~ORBDetector() {}

} // namespace fs
