#pragma once

#include "reader.hpp"
#include "toolbox/base/base.hpp"

#include <opencv2/core/mat.hpp>
#include <opencv2/videoio.hpp>

namespace ct {

enum class VideoSourceType : u8 { File, Camera, Stream };

class VideoReader final : public Reader {
public:
    VideoReader(cv::VideoCapture cap, u64 totalFrames, double fps, VideoSourceType source);
    ~VideoReader() override;

    [[nodiscard]] result<FrameData> Next() override;
    [[nodiscard]] bool HasNext() const override;
    [[nodiscard]] u64 Size() const override;
    void Reset() override;

    [[nodiscard]] static result<ref<VideoReader>> Open(const std::filesystem::path& path);

private:
    [[nodiscard]] static bool IsCameraIndex(const std::string& path);
    [[nodiscard]] static bool IsStreamUrl(const std::string& path);

    cv::VideoCapture mCap;
    u64 mCurrentIndex{0};
    u64 mTotalFrames{0};
    double mFps{30.0};
    Timestamp mStartTime{0.0};
    VideoSourceType mSource;
};

} // namespace ct
