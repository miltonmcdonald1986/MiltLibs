#include <graphics/app/app_guard.h>

#include <graphics/app/lifecycle.h>

using graphics::app::lifecycle::Shutdown;

namespace graphics::app::app_guard
{
	
	AppGuard::AppGuard(app::App& a)
		: app(a)
	{
	}

	AppGuard::~AppGuard()
	{
		Shutdown(app);
	}

}
