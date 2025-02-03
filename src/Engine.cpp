#include "Engine.h"
#include "spdlog/spdlog.h"
#include <stdexcept>

Engine* Engine::instance = nullptr;
std::mutex Engine::mtx;
EventDispatcher* Engine::eventDispatcher = nullptr;
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

    spdlog::info("Engine initialised successfully");
}

void Engine::set_event_dispatcher(EventDispatcher* dispatcher)
{
    eventDispatcher = dispatcher;
}
