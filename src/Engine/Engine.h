#pragma once
#include "Core.h"
#include <mutex>
#include "Events/EventSystem.h"
#include "Window.h"
#include <memory>

#include "Engine/InputManager.h"
#include "Engine/TimeManager.h"

#include "Rendering/OpenGLRenderer.h"

class Engine
{
public:
	Engine(const Engine& obj) = delete;

	static Engine& get_engine();

	void init();
	void shutdown();

	void start_main_loop();

	void set_primary_window(Window* w) { primaryWindow.reset(w); }
	Window& get_primary_window();

	static EventSystem& events();
	static InputManager& input();
	static TimeManager& time();
	static OpenGLRenderer& renderer();

private:
	Engine();

	static std::unique_ptr<Window> primaryWindow;
	static std::unique_ptr<EventSystem> eventsystem;

	static std::unique_ptr<InputManager> inputManager;
	static std::unique_ptr<TimeManager> timeManager;

	static std::unique_ptr<OpenGLRenderer> m_Renderer;
};
