#include "fslam/io/reader.hpp"
#include "fslam/core/logger.hpp"
#include "fslam/io/video_reader.hpp"
#include <filesystem>

namespace fs {

result<ref<Reader>> Reader::Create(const ReaderInfo& info) {
    if (!std::filesystem::exists(info.path))
        return err(ErrorCode::INVALID_ARGUMENT, "Path does not exist: " + info.path.string());

    if (std::filesystem::is_directory(info.path))
        return err(ErrorCode::INVALID_ARGUMENT, "Directory reader not implemented yet");

    auto videoResult = VideoReader::Open(info.path);
    if (!videoResult.has_value()) return err(videoResult.error());

    return ok(ref<Reader>(std::move(videoResult.value())));
}

Reader::Iterator Reader::begin() {
    Reset();
    return Iterator(this);
}

Reader::Iterator Reader::end() { return Iterator(); }

} // namespace fs
