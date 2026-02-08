#pragma once

#include "fslam/io/reader.hpp"
#include <opencv2/core/mat.hpp>
#include <opencv2/videoio.hpp>

namespace fs {

class VideoReader final : public Reader {
public:
    explicit VideoReader(cv::VideoCapture cap, u64 totalFrames, double fps);
    ~VideoReader() override;

    [[nodiscard]] result<FrameData> Next() override;
    [[nodiscard]] bool HasNext() const override;
    [[nodiscard]] u64 Size() const override;
    void Reset() override;

    [[nodiscard]] static result<ref<VideoReader>> Open(const std::filesystem::path& path);

private:
    cv::VideoCapture mCap;
    u64 mCurrentIndex{0};
    u64 mTotalFrames{0};
    double mFps{30.0};
    Timestamp mStartTime{0.0};
};

} // namespace fs
