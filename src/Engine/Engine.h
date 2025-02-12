#pragma once
#include <mutex>
#include "events/EventDispatcher.h"
#include "Window.h"
#include "Core.h"

#include "Engine/InputManager.h"

class Engine
{
public:
	Engine(const Engine& obj) = delete;

	static Engine* get_engine();

	void init();
	void shutdown();

	void start_main_loop();

	void set_event_dispatcher(EventDispatcher* dispatcher);
	EventDispatcher* get_event_dispatcher();

	void set_primary_window(Window* w) { primaryWindow = w; }
	Window* get_primary_window();

	InputManager* get_input_manager();

private:
	Engine();

	static Engine* instance;
	static std::mutex mtx;

	static Window* primaryWindow;
	static EventDispatcher* eventDispatcher;

	static InputManager* inputManager;
};
