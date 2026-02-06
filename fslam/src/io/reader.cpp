#include "fslam/io/reader.hpp"
#include <filesystem>
#include "fslam/io/video_reader.hpp"

#include <fslam/core/logger.hpp>


namespace fs {
ref<Reader> Reader::Create(const ReaderInfo& info)
{

    if (std::filesystem::is_directory(info.path)) {
        log::Critical("Directory reader not implemented yet.");
        std::abort();
    } else {
        return std::make_shared<VideoReader>(info);
    }


}



Reader::Iterator Reader::begin() {
    Reset();
    return Iterator(this);
}

Reader::Iterator Reader::end() {
    return Iterator();
}





}
