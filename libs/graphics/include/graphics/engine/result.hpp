#ifndef GRAPHICS_ENGINE_RESULT_HPP
#define GRAPHICS_ENGINE_RESULT_HPP

#include <expected>

#include <graphics/engine/error.hpp>

// NOLINTBEGIN(cppcoreguidelines-macro-usage)

#define UNEXPECTED(cat, msg) \
    ([&]() { \
        graphics::engine::ErrorInfo _err{ cat, msg, __FILE__, __LINE__ }; \
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

// NOLINTEND(cppcoreguidelines-macro-usage)

namespace graphics::engine
{

	template<typename T>
	using Result = std::expected<T, ErrorInfo>;

	using Status = std::expected<void, ErrorInfo>;

} // namespace graphics::engine

#endif // GRAPHICS_ENGINE_RESULT_HPP