#include "toolbox/vision/io/video.hpp"
#include "toolbox/base/base.hpp"
#include <algorithm>
#include <cctype>

namespace ct {

VideoReader::VideoReader(cv::VideoCapture cap, u64 totalFrames, double fps, VideoSourceType source)
    : mCap(std::move(cap)), mTotalFrames(totalFrames), mFps(fps), mSource(source) {}

VideoReader::~VideoReader() { mCap.release(); }

bool VideoReader::IsCameraIndex(const std::string& path) {
    if (path.empty()) return false;
    return std::all_of(path.begin(), path.end(), [](char c) { return std::isdigit(c); });
}

bool VideoReader::IsStreamUrl(const std::string& path) {
    return path.starts_with("http://") || path.starts_with("https://") ||
           path.starts_with("rtsp://") || path.starts_with("rtmp://");
}

result<ref<VideoReader>> VideoReader::Open(const std::filesystem::path& path) {
    cv::VideoCapture cap;
    std::string pathStr = path.string();
    VideoSourceType source;

    if (IsCameraIndex(pathStr)) {
        int deviceIndex = std::stoi(pathStr);
        cap.open(deviceIndex);
        source = VideoSourceType::Camera;

        if (!cap.isOpened())
            return err(ErrorCode::FAILED_TO_AQUIRE_RESOURCE,
                "Failed to open camera device: " + std::to_string(deviceIndex));

        double fps = cap.get(cv::CAP_PROP_FPS);
        if (fps <= 0.0) fps = 30.0;

        log::Info("Opened camera device {} (fps: {:.1f})", deviceIndex, fps);
        return ok(
            createRef<VideoReader>(std::move(cap), std::numeric_limits<u64>::max(), fps, source));
    }

    if (IsStreamUrl(pathStr)) {
        cap.open(pathStr);
        source = VideoSourceType::Stream;

        if (!cap.isOpened())
            return err(ErrorCode::FAILED_TO_AQUIRE_RESOURCE, "Failed to open stream: " + pathStr);

        double fps = cap.get(cv::CAP_PROP_FPS);
        if (fps <= 0.0) fps = 30.0;

        log::Info("Opened stream: {} (fps: {:.1f})", pathStr, fps);
        return ok(
            createRef<VideoReader>(std::move(cap), std::numeric_limits<u64>::max(), fps, source));
    }

    if (!std::filesystem::exists(path))
        return err(ErrorCode::INVALID_ARGUMENT, "Video file does not exist: " + pathStr);

    cap.open(pathStr);
    source = VideoSourceType::File;

    if (!cap.isOpened())
        return err(ErrorCode::FAILED_TO_AQUIRE_RESOURCE, "Failed to open video: " + pathStr);

    u64 totalFrames = static_cast<u64>(cap.get(cv::CAP_PROP_FRAME_COUNT));
    double fps = cap.get(cv::CAP_PROP_FPS);
    if (fps <= 0.0) fps = 30.0;

    log::Info("Opened video file: {} ({} frames, {:.1f} fps)", pathStr, totalFrames, fps);
    return ok(createRef<VideoReader>(std::move(cap), totalFrames, fps, source));
}

result<Reader::FrameData> VideoReader::Next() {
    if (!HasNext()) return err(ErrorCode::INVALID_ARGUMENT, "No more frames to read");

    cv::Mat frame;
    mCap.read(frame);

    if (frame.empty()) {
        if (mSource == VideoSourceType::Camera)
            return err(ErrorCode::UNKNOWN_ERROR, "Camera returned empty frame");
        if (mSource == VideoSourceType::Stream)
            return err(ErrorCode::UNKNOWN_ERROR, "Stream returned empty frame");
        return err(ErrorCode::UNKNOWN_ERROR,
            "Failed to read frame at index " + std::to_string(mCurrentIndex));
    }

    Timestamp ts = mStartTime + (static_cast<double>(mCurrentIndex) / mFps);
    mCurrentIndex++;

    return ok(FrameData{std::move(frame), ts});
}

bool VideoReader::HasNext() const {
    if (!mCap.isOpened()) return false;

    // NOTE: Camera and stream are infinite sources
    if (mSource == VideoSourceType::Camera || mSource == VideoSourceType::Stream) return true;

    return mCurrentIndex < mTotalFrames;
}

u64 VideoReader::Size() const { return mTotalFrames; }

void VideoReader::Reset() {
    if (mSource == VideoSourceType::File) {
        mCurrentIndex = 0;
        mCap.set(cv::CAP_PROP_POS_FRAMES, 0);
    }
    // NOTE: Can't rewind a live camera or network stream
}

} // namespace ct
