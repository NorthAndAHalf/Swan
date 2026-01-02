#include "Engine.h"
#include "spdlog/spdlog.h"
#include <stdexcept>
#include "Renderer/RenderPipelines/BasicPipeline.h"

Engine* Engine::instance = nullptr;
std::mutex Engine::mtx;

EventSystem* Engine::eventsystem = nullptr;
Window* Engine::primaryWindow = nullptr;
InputManager* Engine::inputManager = nullptr;
TimeManager* Engine::timeManager = nullptr;
Renderer* Engine::m_Renderer = nullptr;

Engine::Engine()
{
}

Engine* Engine::get_engine()
{
    if (instance == nullptr) {
        std::lock_guard<std::mutex> lock(mtx);
        if (instance == nullptr) {
            instance = new Engine();
        }
    }

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
    
    eventsystem = new EventSystem();
    eventsystem->init();

    primaryWindow = new Window("Snowfall", 1080, 1920, false);
    primaryWindow->init();

    inputManager = new InputManager(primaryWindow);
    inputManager->init();

    timeManager = new TimeManager();

    m_Renderer = new Renderer();
    m_Renderer->set_pipeline(std::make_shared<BasicPipeline>());

    spdlog::info("Engine initialised successfully");
}

void Engine::shutdown()
{
    spdlog::info("Shutting down Engine");
    delete m_Renderer;
    delete timeManager;
    delete inputManager;
    primaryWindow->destroy();
    delete primaryWindow;
    delete eventsystem;
    glfwTerminate();
}

void Engine::start_main_loop()
{
    while (!primaryWindow->window_should_close())
    {
        eventsystem->dispatch_queued_events();
        eventsystem->fire_event<FrameStartEvent>();

        eventsystem->fire_event<UpdateEvent>();
        m_Renderer->update();
        primaryWindow->update();

        eventsystem->fire_event<FrameEndEvent>();
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

Renderer& Engine::renderer()
{
    SF_ASSERT(m_Renderer, "Renderer is null");
    return *m_Renderer;
}
