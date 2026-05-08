#ifndef GRAPHICS_ENGINE_ERROR_INFO_HPP
#define GRAPHICS_ENGINE_ERROR_INFO_HPP

#include <memory>
#include <string>

#include <spdlog/logger.h>

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
        std::string message;
        std::string file;
        int line{ 0 };
    };

    auto log_error(const ErrorInfo& err) -> void;
    auto logger() -> Logger&;

} // namespace graphics::engine

#endif // GRAPHICS_ENGINE_ERROR_INFO_HPP