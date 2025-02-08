#pragma once
#include <mutex>
#include "events/EventDispatcher.h"
#include "Window.h"
#include "Core.h"

class Engine
{
public:
	Engine(const Engine& obj) = delete;

	static Engine* get_engine();

	void init();

	void set_event_dispatcher(EventDispatcher* dispatcher);
	EventDispatcher& get_event_dispatcher() 
	{ 
		ENGINE_ASSERT(eventDispatcher, "Engine event dispatcher is null");
		return *eventDispatcher; 
	}

	void set_primary_window(Window* w) { primaryWindow = w; }
	Window* get_primary_window() { return primaryWindow; }

	// Input Polling (could move into an input class in the future)
	bool get_key(int keycode);
	bool get_mouse_button(int button);
	std::pair<double, double> get_cursor();
	double get_cursorX();
	double get_cursorY();

	void block_inputs();
	void unblock_inputs();

private:
	Engine() {}

	static Engine* instance;
	static std::mutex mtx;

	static Window* primaryWindow;

	static EventDispatcher* eventDispatcher;

	static bool isBlockingInputs;
};
