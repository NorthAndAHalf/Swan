#include "Engine.h"
#include "spdlog/spdlog.h"
#include <stdexcept>
#include "Renderer/RenderPipelines/BasicPipeline.h"

Engine* Engine::instance = nullptr;
std::mutex Engine::mtx;

EventDispatcher* Engine::eventDispatcher = nullptr;
Window* Engine::primaryWindow = nullptr;
InputManager* Engine::inputManager = nullptr;
TimeManager* Engine::timeManager = nullptr;
Renderer* Engine::renderer = nullptr;

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
    spdlog::info("Initialising Engine");

    spdlog::info("Initialising GLFW");
    if (!glfwInit())
    {
        throw std::runtime_error("Failed to initialise GLFW");
    }

    eventDispatcher = new EventDispatcher();

    primaryWindow = new Window("Snowfall", 1080, 1920, false);
    primaryWindow->init();

    inputManager = new InputManager(primaryWindow);
    timeManager = new TimeManager();

    renderer = new Renderer();
    renderer->set_pipeline(std::make_shared<BasicPipeline>());

    spdlog::info("Engine initialised successfully");
}

void Engine::shutdown()
{
    spdlog::info("Shutting down engine");
    delete inputManager;
}

void Engine::start_main_loop()
{
    while (!primaryWindow->window_should_close())
    {
        eventDispatcher->dispatch_frame_start();

        eventDispatcher->dispatch_update();
        renderer->update();
        primaryWindow->update();

        eventDispatcher->dispatch_frame_end();
    }
}

EventDispatcher& Engine::get_event_dispatcher()
{
    SF_ASSERT(eventDispatcher, "Event dispatcher is null");
    return *eventDispatcher;
}

Window& Engine::get_primary_window()
{
    SF_ASSERT(primaryWindow, "Primary window is null");
    return *primaryWindow;
}

InputManager& Engine::get_input_manager()
{
    SF_ASSERT(inputManager, "Input manager is null");
    return *inputManager;
}

TimeManager& Engine::get_time_manager()
{
    SF_ASSERT(timeManager, "Time manager is null");
    return *timeManager;
}