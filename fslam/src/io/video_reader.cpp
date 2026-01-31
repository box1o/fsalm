#include "fslam/io/video_reader.hpp"
#include <fslam/core/logger.hpp>

namespace fs {

VideoReader::VideoReader(const ReaderInfo& info) {
    mCap.open(info.path.string());

    if (!mCap.isOpened()) {
        log::Critical("Failed to open video: {}", info.path.string());
        std::abort();
    }

    mTotalFrames = static_cast<std::size_t>(mCap.get(cv::CAP_PROP_FRAME_COUNT));
    mFps = mCap.get(cv::CAP_PROP_FPS);
}
VideoReader::~VideoReader() { mCap.release(); }

Reader::FrameData VideoReader::Next() {
    if (!HasNext()) {
        log::Critical("No more frames to read");
        std::abort();
    }

    mCap.read(mCurrentFrame);

    if (mCurrentFrame.empty()) {
        log::Critical("Failed to read frame at index {}", mCurrentIndex);
        std::abort();
    }

    Timestamp ts = mStartTime + (static_cast<double>(mCurrentIndex) / mFps);
    mCurrentIndex++;

    return {mCurrentFrame, ts};
}

void VideoReader::Reset() {
    mCurrentIndex = 0;
    mCap.set(cv::CAP_PROP_POS_FRAMES, 0);
}

bool VideoReader::HasNext() { return mCap.isOpened() && mCurrentIndex < mTotalFrames; }
u64 VideoReader::Size() const { return mTotalFrames; }

} // namespace fs
