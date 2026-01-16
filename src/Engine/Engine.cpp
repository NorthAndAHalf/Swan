#include "Engine.h"
#include "spdlog/spdlog.h"
#include <stdexcept>
#include "Rendering/OpenGL/OpenGLRenderer.h"

std::unique_ptr<EventSystem> Engine::eventsystem = nullptr;
std::unique_ptr<Window> Engine::primaryWindow = nullptr;
std::unique_ptr<InputManager> Engine::inputManager = nullptr;
std::unique_ptr<TimeManager> Engine::timeManager = nullptr;
std::unique_ptr<OpenGLRenderer> Engine::m_Renderer = nullptr;

Engine::Engine()
{
}

Engine& Engine::get_engine()
{
    static Engine instance;
    return instance;
}

void Engine::init()
{
    spdlog::info("Starting Snowfall");

    // Move to static function in window class
    spdlog::info("Initialising GLFW");
    if (!glfwInit())
    {
        throw std::runtime_error("Failed to initialise GLFW");
    }
    
    eventsystem = std::make_unique<EventSystem>();
    eventsystem->init();

    primaryWindow = std::make_unique<Window>("Snowfall", 1920, 1080, false);
    primaryWindow->init();

    inputManager = std::make_unique<InputManager>(primaryWindow.get());
    inputManager->init();

    timeManager = std::make_unique<TimeManager>();

    m_Renderer = std::make_unique<OpenGLRenderer>();
    m_Renderer->init();

    spdlog::info("Engine initialised successfully");
}

void Engine::shutdown()
{
    spdlog::info("Shutting down Engine");
    primaryWindow->destroy();
    glfwTerminate();
}

void Engine::start_main_loop()
{
    while (!primaryWindow->window_should_close())
    {
        m_Renderer->swap_buffers();
        eventsystem->dispatch_queued_events();
        m_Renderer->update();
        eventsystem->fire_event<UpdateEvent>();
        primaryWindow->update();
    }
}

Window& Engine::get_primary_window()
{
    SF_ASSERT(primaryWindow, "Primary window is null");
    return *primaryWindow;
}

EventSystem& Engine::events()
{
    SF_ASSERT(eventsystem, "Event system is null");
    return *eventsystem;
}

InputManager& Engine::input()
{
    SF_ASSERT(inputManager, "Input is null");
    return *inputManager;
}

TimeManager& Engine::time()
{
    SF_ASSERT(timeManager, "Time manager is null");
    return *timeManager;
}

OpenGLRenderer& Engine::renderer()
{
    SF_ASSERT(m_Renderer, "Renderer is null");
    return *m_Renderer;
}
