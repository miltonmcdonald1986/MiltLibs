#ifndef GRAPHICS_APP_LIFECYCLE_H
#define GRAPHICS_APP_LIFECYCLE_H

#include "app.h"

#include <expected>

namespace graphics::app::lifecycle
{

    std::expected<void, std::string> InitGLContext(app::App& app);
    std::expected<void, std::string> InitGLState(app::App& app);
    std::expected<void, std::string> InitPlatform(app::App& app);
    void Shutdown(app::App& app);

}

#endif // GRAPHICS_APP_LIFECYCLE_H
