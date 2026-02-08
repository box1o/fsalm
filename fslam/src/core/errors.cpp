#include "fslam/core/errors.hpp"
#include "fslam/core/logger.hpp"

namespace fs {

void Error::Log() const {
    log::Error("[{}:{}] ErrorCode={} — {}", mLocation.file_name(), mLocation.line(),
        static_cast<u16>(mCode), mMessage);
}

} // namespace fs
