#pragma once

#include "fslam/io/reader.hpp"
#include <opencv2/core/mat.hpp>
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
    cv::VideoCapture mCap;
    std::size_t mCurrentIndex{0};
    std::size_t mTotalFrames{0};
    double mFps{30.0};
    Timestamp mStartTime{0.0};
    cv::Mat mCurrentFrame;
};

} // namespace fs
