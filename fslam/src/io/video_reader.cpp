#include "fslam/io/video_reader.hpp"
#include "fslam/core/logger.hpp"

namespace fs {

VideoReader::VideoReader(cv::VideoCapture cap, u64 totalFrames, double fps)
    : mCap(std::move(cap)), mTotalFrames(totalFrames), mFps(fps) {}

VideoReader::~VideoReader() { mCap.release(); }

result<ref<VideoReader>> VideoReader::Open(const std::filesystem::path& path) {
    cv::VideoCapture cap;
    cap.open(path.string());

    if (!cap.isOpened())
        return err(ErrorCode::CREATE_PIPELINE_FAILED, "Failed to open video: " + path.string());

    u64 totalFrames = static_cast<u64>(cap.get(cv::CAP_PROP_FRAME_COUNT));
    double fps = cap.get(cv::CAP_PROP_FPS);

    if (fps <= 0.0) fps = 30.0;

    return ok(createRef<VideoReader>(std::move(cap), totalFrames, fps));
}

result<Reader::FrameData> VideoReader::Next() {
    if (!HasNext()) return err(ErrorCode::INVALID_ARGUMENT, "No more frames to read");

    cv::Mat frame;
    mCap.read(frame);

    if (frame.empty())
        return err(ErrorCode::UNKNOWN_ERROR,
            "Failed to read frame at index " + std::to_string(mCurrentIndex));

    Timestamp ts = mStartTime + (static_cast<double>(mCurrentIndex) / mFps);
    mCurrentIndex++;

    return ok(FrameData{std::move(frame), ts});
}

bool VideoReader::HasNext() const { return mCap.isOpened() && mCurrentIndex < mTotalFrames; }

u64 VideoReader::Size() const { return mTotalFrames; }

void VideoReader::Reset() {
    mCurrentIndex = 0;
    mCap.set(cv::CAP_PROP_POS_FRAMES, 0);
}

} // namespace fs
