#include "spdlog/spdlog.h"
#include "Engine.h"
#include "events/EventListener.h"
#include "window.h"
#include "services/GameService.h"

#ifdef SD_DEBUG
#define SPD_DEBUG_LEVEL(x) x;
#else
#define SPD_DEBUG_LEVEL(x);
#endif

void setup()
{
	EventDispatcher* eventDispatcher = new EventDispatcher();
	Engine::get_engine()->set_event_dispatcher(eventDispatcher);

	if (!glfwInit())
	{
		throw std::runtime_error("Failed to initialise GLFW");
		return;
	}
	spdlog::info("Initialised GLFW");

	Engine::get_engine()->init();
}

int main()
{
	// Setting up spdlog level, spdlog::trace() will only print when in debug configuration, other log functions will work in all configurations
	SPD_DEBUG_LEVEL(spdlog::set_level(spdlog::level::trace));
	
	try
	{
		setup();
	}
	catch (const std::exception& e)
	{
		spdlog::critical(e.what());
		return -1;
	}

	GameService gameService = GameService();
	Engine::get_engine()->get_event_dispatcher().subscribe(&gameService);

	while (!Engine::get_engine()->get_primary_window()->window_should_close())
	{
		Engine::get_engine()->get_primary_window()->update();
		gameService.update();
	}
}
