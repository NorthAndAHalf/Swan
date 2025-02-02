#include "spdlog/spdlog.h"
#include "Engine.h"

void setup()
{
	EventDispatcher* eventDispatcher = new EventDispatcher();
	Engine::get_engine()->set_event_dispatcher(eventDispatcher);

	if (!glfwInit())
	{
		spdlog::critical("Failed to initialise GLFW");
		return;
	}
	spdlog::info("Initialised GLFW");

	Engine::get_engine()->init();
}

int main()
{
	try
	{
		setup();
	}
	catch (const std::exception& e)
	{
		spdlog::error("Engine Initialisation Error: {0}", e.what());
		return -1;
	}

	while (!Engine::get_engine()->get_primary_window()->window_should_close())
	{
		Engine::get_engine()->get_primary_window()->update();
	}
}
