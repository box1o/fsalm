#include "fslam/io/video_reader.hpp"
#include <fslam/core/logger.hpp>

namespace fs {

VideoReader::VideoReader(const ReaderInfo& info) {}
VideoReader::~VideoReader() {}

Reader::FrameData VideoReader::Next() {
    log::Warn("VideoReader::Next()");
    return {};
}

bool VideoReader::HasNext() {

    return true;
}

u64 VideoReader::Size() const {

    return 64;
}


void VideoReader::Reset() {
    log::Warn("VideoReader::Reset()");
}

} // namespace fs
