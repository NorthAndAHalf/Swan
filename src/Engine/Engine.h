#pragma once
#include "Core.h"
#include <mutex>
#include "Events/EventSystem.h"
#include "Window.h"
#include <memory>

#include "Engine/InputManager.h"
#include "Engine/TimeManager.h"

#include "Rendering/OpenGL/OpenGLRenderer.h"

class Engine
{
public:
	Engine(const Engine& obj) = delete;

	static Engine& GetEngine();

	void Init();
	void Shutdown();

	void MainLoop();

	void SetPrimaryWindow(Window* w) { primaryWindow.reset(w); }
	Window& GetPrimaryWindow();

	static EventSystem& Events();
	static InputManager& Input();
	static TimeManager& Time();
	static OpenGLRenderer& Renderer();

private:
	Engine();

	static std::unique_ptr<Window> primaryWindow;
	static std::unique_ptr<EventSystem> eventsystem;

	static std::unique_ptr<InputManager> inputManager;
	static std::unique_ptr<TimeManager> timeManager;

	static std::unique_ptr<OpenGLRenderer> m_Renderer;
};
