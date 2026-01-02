#pragma once
#include "Core.h"
#include <mutex>
#include "Events/EventSystem.h"
#include "Window.h"

#include "Engine/InputManager.h"
#include "Engine/TimeManager.h"

#include "Renderer/Renderer.h"

class Engine
{
public:
	Engine(const Engine& obj) = delete;

	static Engine* get_engine();

	void init();
	void shutdown();

	void start_main_loop();

	EventSystem& get_event_system();

	void set_primary_window(Window* w) { primaryWindow = w; }
	Window& get_primary_window();

	InputManager& get_input_manager();
	TimeManager& get_time_manager();

private:
	Engine();

	static Engine* instance;
	static std::mutex mtx;

	static Window* primaryWindow;
	static EventSystem* eventsystem;

	static InputManager* inputManager;
	static TimeManager* timeManager;

	static Renderer* renderer;
};
