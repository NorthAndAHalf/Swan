#include "Engine.h"
#include "spdlog/spdlog.h"
#include <stdexcept>

Engine* Engine::instance = nullptr;
std::mutex Engine::mtx;

EventDispatcher* Engine::eventDispatcher = nullptr;
Window* Engine::primaryWindow = nullptr;
InputManager* Engine::inputManager = nullptr;
TimeManager* Engine::timeManager = nullptr;

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

    primaryWindow = new Window("Snowdrift", 1080, 1920, false);
    primaryWindow->init();

    inputManager = new InputManager(primaryWindow);
    timeManager = new TimeManager();

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
        primaryWindow->update();

        eventDispatcher->dispatch_frame_end();
    }
}

EventDispatcher& Engine::get_event_dispatcher()
{
    ENGINE_ASSERT(eventDispatcher, "Event dispatcher is null");
    return *eventDispatcher;
}

Window& Engine::get_primary_window()
{
    ENGINE_ASSERT(primaryWindow, "Primary window is null");
    return *primaryWindow;
}

InputManager& Engine::get_input_manager()
{
    ENGINE_ASSERT(inputManager, "Input manager is null");
    return *inputManager;
}

TimeManager& Engine::get_time_manager()
{
    ENGINE_ASSERT(timeManager, "Time manager is null");
    return *timeManager;
}