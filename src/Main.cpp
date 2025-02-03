#include "spdlog/spdlog.h"
#include "Engine.h"
#include "Events/EventListener.h"
#include "window.h"

void setup(Window* primaryWindow)
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
	
	primaryWindow->init();
}

// Initial design contained a circular dependency on window and engine so this is a quick fix
// Not sure how to refactor this yet, maybe an application class that can contain high level class like engine and window
int main()
{
	Window primaryWindow = Window("Snowdrift", 1080, 1920, false);
	try
	{
		setup(&primaryWindow);
	}
	catch (const std::exception& e)
	{
		spdlog::error("Engine Initialisation Error: {0}", e.what());
		return -1;
	}

	while (!primaryWindow.window_should_close())
	{
		primaryWindow.update();
	}
}
