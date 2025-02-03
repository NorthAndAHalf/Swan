#pragma once
#include <mutex>
#include "Events/EventDispatcher.h"
#include "Window.h"

class Engine
{
public:
	Engine(const Engine& obj) = delete;

	static Engine* get_engine();

	void init();

	void set_event_dispatcher(EventDispatcher* dispatcher);
	EventDispatcher& get_event_dispatcher() { return *eventDispatcher; }

	void set_primary_window(Window* w) { primaryWindow = w; }
	Window* get_primary_window() { return primaryWindow; }

private:
	Engine() {}

	static Engine* instance;
	static std::mutex mtx;

	static Window* primaryWindow;

	static EventDispatcher* eventDispatcher;
};
