#pragma once

#include "fslam/types.hpp"
#include <source_location>
#include <string>
#include <string_view>

namespace fs {

enum class ErrorType : u8 { CORE = 0, PIPELINE, ARGUMENT, FEATURE, VALIDATION, GRAPHICS };

enum class ErrorCode : u16 {
    SUCCESS = 0,

    CREATE_PIPELINE_FAILED = 100,

    INVALID_ARGUMENT = 200,

    NOT_ENOUGH_FEATURES = 300,

    UNKNOWN_ERROR = 999
};

class Error {
public:
    constexpr Error(ErrorCode code, std::string_view msg,
        std::source_location loc = std::source_location::current()) noexcept
        : mCode(code), mMessage(msg), mLocation(loc) {}

    [[nodiscard]] constexpr ErrorCode Code() const noexcept { return mCode; }

    [[nodiscard]] constexpr ErrorType Type() const noexcept {
        const u16 val = static_cast<u16>(mCode);
        if (val >= 100 && val < 200) return ErrorType::PIPELINE;
        if (val >= 200 && val < 300) return ErrorType::ARGUMENT;
        if (val >= 300 && val < 400) return ErrorType::FEATURE;
        if (val >= 400 && val < 500) return ErrorType::VALIDATION;
        if (val >= 500 && val < 600) return ErrorType::GRAPHICS;
        return ErrorType::CORE;
    }

    [[nodiscard]] constexpr std::string_view Message() const noexcept { return mMessage; }
    [[nodiscard]] constexpr const std::source_location& Location() const noexcept {
        return mLocation;
    }

    void Log() const;

private:
    ErrorCode mCode;
    std::string mMessage;
    std::source_location mLocation;
};

} // namespace fs
