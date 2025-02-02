#pragma once
#include <mutex>
#include "EventDispatcher.h"
#include "Window.h"

class Engine
{
public:
	Engine(const Engine& obj) = delete;

	static Engine* get_engine();

	void init();

	void set_event_dispatcher(EventDispatcher* dispatcher);
	void set_primary_window(Window* window);
	Window* get_primary_window() { return primaryWindow; }

private:
	Engine() {}

	static Engine* instance;
	static std::mutex mtx;

	static EventDispatcher* eventDispatcher;
	static bool isEventDispatcherSet;

	static Window* primaryWindow;
};