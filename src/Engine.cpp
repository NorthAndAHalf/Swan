#include "Engine.h"
#include "spdlog/spdlog.h"
#include <stdexcept>

Engine* Engine::instance = nullptr;
std::mutex Engine::mtx;
EventDispatcher* Engine::eventDispatcher = nullptr;
bool Engine::isEventDispatcherSet = false;
Window* Engine::primaryWindow = nullptr;

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
    spdlog::info("Initialising Snowdrift");

    if (!isEventDispatcherSet)
    {
        throw std::runtime_error("Engine event dispatcher not set");
    }

    if (!primaryWindow)
    {
        spdlog::info("Using default primary window");
        primaryWindow = new Window("Snowdrift", 1080, 1920, false);
        primaryWindow->init();
    }

    spdlog::info("Snowdrift initialised successfully");
}

void Engine::set_event_dispatcher(EventDispatcher* dispatcher)
{
    eventDispatcher = dispatcher;
    isEventDispatcherSet = true;
}

void Engine::set_primary_window(Window* window)
{
    primaryWindow = window;
}