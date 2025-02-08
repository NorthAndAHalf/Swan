#include "Engine.h"
#include "spdlog/spdlog.h"
#include <stdexcept>

Engine* Engine::instance = nullptr;
std::mutex Engine::mtx;

EventDispatcher* Engine::eventDispatcher = nullptr;
Window* Engine::primaryWindow = nullptr;
InputManager* Engine::inputManager = nullptr;

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

    if (!eventDispatcher)
    {
        throw std::runtime_error("Engine event dispatcher not set");
    }

    if (!primaryWindow)
    {
        spdlog::trace("No primary window set for Engine, using default");
        primaryWindow = new Window("Snowdrift", 1080, 1920, false);
        primaryWindow->init();
    }

    inputManager = new InputManager(primaryWindow);

    spdlog::info("Engine initialised successfully");
}

void Engine::shutdown()
{
    delete inputManager;
}

void Engine::set_event_dispatcher(EventDispatcher* dispatcher)
{
    eventDispatcher = dispatcher;
}

EventDispatcher* Engine::get_event_dispatcher()
{
    ENGINE_ASSERT(eventDispatcher, "Event dispatcher is null");
    return eventDispatcher;
}

Window* Engine::get_primary_window()
{
    ENGINE_ASSERT(primaryWindow, "Primary window is null");
    return primaryWindow;
}

InputManager* Engine::get_input_manager()
{
    ENGINE_ASSERT(inputManager, "Input manager is null");
    return inputManager;
}
