#include <graphics/engine/error.h>

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/ringbuffer_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace graphics::engine
{

    static constexpr std::string_view PROJECT_ROOT = "MiltLibs";

    void log_error(const ErrorInfo& err)
    {
        engine::logger()->error("[{}] {} ({}:{})", magic_enum::enum_name(err.category), err.message, err.file, err.line);
    }

    auto logger() -> Logger&
    {
        static Logger instance = []
            {
                // Create sinks
                auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
                console_sink->set_pattern("[%T] [%^%l%$] %v");

                auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("graphics.log", true);
                file_sink->set_pattern("[%Y-%m-%d %T] [%l] %v");

                auto ringbuffer_sink = std::make_shared<spdlog::sinks::ringbuffer_sink_mt>(2000);
                ringbuffer_sink->set_pattern("[%l] %v");

                // Combine sinks
                auto logger = std::make_shared<spdlog::logger>("graphics", spdlog::sinks_init_list{ console_sink, file_sink, ringbuffer_sink });

                // Configure logger
                logger->set_level(spdlog::level::debug);   // or info in release
                logger->flush_on(spdlog::level::err);

                return logger;
            }();

        return instance;
    }

}
