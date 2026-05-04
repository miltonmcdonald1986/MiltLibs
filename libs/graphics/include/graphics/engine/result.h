#ifndef GRAPHICS_ENGINE_RESULT_H
#define GRAPHICS_ENGINE_RESULT_H

#include <expected>

#include <graphics/engine/error.h>

#if defined(DEBUG) || defined(_DEBUG)
    #define CAPTURE_TRACE std::stacktrace::current()
#else
    #define CAPTURE_TRACE std::stacktrace{}
#endif

#define UNEXPECTED(cat, msg) \
    ([&]() { \
        graphics::engine::ErrorInfo _err{ cat, msg, __FILE__, __LINE__, CAPTURE_TRACE }; \
        graphics::engine::log_error(_err); \
        return std::unexpected(_err); \
    }())

#define VALUE_OR_RETURN_UNEXPECTED(expr)                     \
    ({ auto _r = (expr);                                     \
       if (!_r) return std::unexpected(_r.error());          \
       *_r; })

#define RETURN_IF_UNEXPECTED(expr) \
    do { \
        auto _r = (expr); \
        if (!_r) \
            return std::unexpected(_r.error()); \
    } while (0)

namespace graphics::engine
{

	template<typename T>
	using Result = std::expected<T, ErrorInfo>;

	using Status = std::expected<void, ErrorInfo>;

}

#endif // GRAPHICS_ENGINE_RESULT_H
