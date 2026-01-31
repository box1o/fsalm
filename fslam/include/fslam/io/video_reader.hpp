#pragma once

#include "fslam/io/reader.hpp"
#include <opencv2/opencv.hpp>

namespace fs {

class VideoReader final : public Reader {
public:
    VideoReader(const ReaderInfo& info);
    ~VideoReader() override;

    FrameData Next() override;
    bool HasNext() override;
    u64 Size() const override;

    void Reset() override;

private:
    cv::VideoCapture cap_;
    std::size_t currentIndex_{0};
    std::size_t totalFrames_{0};
    double fps_{30.0};
    Timestamp startTime_{0.0};
};

} // namespace fs
