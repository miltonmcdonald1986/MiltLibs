#ifndef GRAPHICS_ENGINE_ERROR_INFO_H
#define GRAPHICS_ENGINE_ERROR_INFO_H

#include <stacktrace>

#include <magic_enum/magic_enum.hpp>

#include <spdlog/spdlog.h>

namespace graphics::engine
{

    using Logger = std::shared_ptr<spdlog::logger>;

    enum class ErrorCategory
    {
        Engine,
        Factories,
        Mesh,
        Platform,
        Rendering,
        Unknown
    };

    struct ErrorInfo
    {
        ErrorCategory category{ ErrorCategory::Unknown };
        std::string message{};
        std::string file{};
        int line{ 0 };
        std::stacktrace trace{}; // empty unless captured
    };

    auto log_error(const ErrorInfo& err) -> void;
    auto logger() -> Logger&;

}

#endif // GRAPHICS_ENGINE_ERROR_INFO_H
