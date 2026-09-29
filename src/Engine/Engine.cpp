#include "Engine.h"
#include "spdlog/spdlog.h"
#include <stdexcept>
#include "Events/EventSystem.h"
#include "Engine/InputManager.h"
#include "Engine/TimeManager.h"
#include "Rendering/OpenGL/OpenGLRenderer.h"

std::unique_ptr<EventSystem> Engine::eventsystem = nullptr;
std::unique_ptr<Window> Engine::primaryWindow = nullptr;
std::unique_ptr<InputManager> Engine::inputManager = nullptr;
std::unique_ptr<TimeManager> Engine::timeManager = nullptr;
std::unique_ptr<OpenGLRenderer> Engine::m_Renderer = nullptr;

Engine::Engine()
{
}

Engine& Engine::GetEngine()
{
    static Engine instance;
    return instance;
}

void Engine::Init()
{
    spdlog::info("Starting Swan");

    // Move to static function in window class
    spdlog::info("Initialising GLFW");
    if (!glfwInit())
    {
        throw std::runtime_error("Failed to initialise GLFW");
    }
    
    eventsystem = std::make_unique<EventSystem>();

    primaryWindow = std::make_unique<Window>("Swan", 1280, 720, false);
    primaryWindow->Init();

    inputManager = std::make_unique<InputManager>(primaryWindow.get());

    timeManager = std::make_unique<TimeManager>();

    m_Renderer = std::make_unique<OpenGLRenderer>(1280, 720);
    m_Renderer->Init();

    imguiService = new ImGuiService();
    gameService = new GameService();

    spdlog::info("Engine initialised successfully");
}

void Engine::Shutdown()
{
    spdlog::info("Shutting down Engine");
    primaryWindow->Destroy();
    glfwTerminate();
}

void Engine::MainLoop()
{

    while (!primaryWindow->WindowShouldClose())
    {
        eventsystem->DispatchQueuedEvents();
        primaryWindow->Update();
        m_Renderer->Update();
        inputManager->Update();
        eventsystem->FireEvent<UpdateEvent>();
    }

    delete gameService;
    delete imguiService;
}

Window& Engine::GetPrimaryWindow()
{
    SW_ASSERT(primaryWindow, "Primary window is null");
    return *primaryWindow;
}

EventSystem& Engine::Events()
{
    SW_ASSERT(eventsystem, "Event system is null");
    return *eventsystem;
}

InputManager& Engine::Input()
{
    SW_ASSERT(inputManager, "Input is null");
    return *inputManager;
}

TimeManager& Engine::Time()
{
    SW_ASSERT(timeManager, "Time manager is null");
    return *timeManager;
}

OpenGLRenderer& Engine::Renderer()
{
    SW_ASSERT(m_Renderer, "Renderer is null");
    return *m_Renderer;
}
