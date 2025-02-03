#pragma once
#include <mutex>
#include "Events/EventDispatcher.h"

class Engine
{
public:
	Engine(const Engine& obj) = delete;

	static Engine* get_engine();

	void init();

	void set_event_dispatcher(EventDispatcher* dispatcher);
	EventDispatcher& get_event_dispatcher() { return *eventDispatcher; }

private:
	Engine() {}

	static Engine* instance;
	static std::mutex mtx;

	static EventDispatcher* eventDispatcher;
	static bool isEventDispatcherSet;
};
