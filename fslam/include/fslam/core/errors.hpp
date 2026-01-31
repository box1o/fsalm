#pragma once

#include <string>
#include <string_view>
#include <source_location>
#include "types.hpp"


namespace fs {

enum class ErrorType : u8 {
    CORE = 0,
    FILE_SYSTEM,
    NETWORK,
    PARSE,
    VALIDATION,
    GRAPHICS
};

enum class ErrorCode : u16 {
    SUCCESS = 0,


    UNKNOWN_ERROR = 999
};

class Error {
public:
    constexpr Error(
        ErrorCode code,
        std::string_view msg,
        std::source_location loc = std::source_location::current()) noexcept
        : mCode(code)
        , mMessage(msg)
        , mLocation(loc) {}

    [[nodiscard]] constexpr ErrorCode Code() const noexcept {
        return mCode;
    }

    [[nodiscard]] constexpr ErrorType Type() const noexcept {
        const u16 code_val = static_cast<u16>(mCode);

        if (code_val >= 100 && code_val < 200) return ErrorType::FILE_SYSTEM;
        if (code_val >= 200 && code_val < 300) return ErrorType::NETWORK;
        if (code_val >= 300 && code_val < 400) return ErrorType::PARSE;
        if (code_val >= 400 && code_val < 500) return ErrorType::VALIDATION;
        if (code_val >= 500 && code_val < 600) return ErrorType::GRAPHICS;
        return ErrorType::CORE;
    }

    [[nodiscard]] constexpr std::string_view Message() const noexcept {
        return mMessage;
    }

    [[nodiscard]] constexpr const std::source_location& Location() const noexcept {
        return mLocation;
    }

    void Log() const;

private:
    ErrorCode mCode;
    std::string mMessage;
    std::source_location mLocation;
};

} // namespace ct
