#include "toolbox/vision/io/reader.hpp"
#include "toolbox/vision/io/video.hpp"
#include <algorithm>
#include <cctype>
#include <filesystem>

namespace ct {

result<ref<Reader>> Reader::Create(const ReaderInfo& info) {
    std::string pathStr = info.path.string();

    bool isCamera = !pathStr.empty() &&
        std::all_of(pathStr.begin(), pathStr.end(), [](char c) { return std::isdigit(c); });

    bool isStream = pathStr.starts_with("http://")
        || pathStr.starts_with("https://")
        || pathStr.starts_with("rtsp://")
        || pathStr.starts_with("rtmp://");

    //NOTE: Only check filesystem for actual file paths
    if (!isCamera && !isStream) {
        if (!std::filesystem::exists(info.path))
            return err(ErrorCode::INVALID_ARGUMENT, "Path does not exist: " + pathStr);

        if (std::filesystem::is_directory(info.path))
            return err(ErrorCode::INVALID_ARGUMENT, "Directory reader not implemented yet");
    }

    auto videoResult = VideoReader::Open(info.path);
    if (!videoResult.has_value())
        return err(videoResult.error());

    return ok(ref<Reader>(std::move(videoResult.value())));
}

Reader::Iterator Reader::begin() {
    Reset();
    return Iterator(this);
}

Reader::Iterator Reader::end() {
    return Iterator();
}

} // namespace fs
