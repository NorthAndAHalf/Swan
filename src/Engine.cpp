#include "Engine.h"
#include "spdlog/spdlog.h"
#include <stdexcept>

Engine* Engine::instance = nullptr;
std::mutex Engine::mtx;
EventDispatcher* Engine::eventDispatcher = nullptr;
Window* Engine::primaryWindow = nullptr;
bool Engine::isBlockingInputs = false;

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

bool Engine::get_key(int keycode)
{
    if (isBlockingInputs) return false;
    return primaryWindow->get_key(keycode);
}

bool Engine::get_mouse_button(int button)
{
    if (isBlockingInputs) return false;
    return primaryWindow->get_mouse_button(button);
}

std::pair<double, double> Engine::get_cursor()
{
    double xpos, ypos;
    primaryWindow->get_cursor(&xpos, &ypos);
    return std::pair<double, double>(xpos, ypos);
}

double Engine::get_cursorX()
{
    double xpos, ypos;
    primaryWindow->get_cursor(&xpos, &ypos);
    return xpos;
}

double Engine::get_cursorY()
{
    double xpos, ypos;
    primaryWindow->get_cursor(&xpos, &ypos);
    return ypos;
}

void Engine::block_inputs()
{
    isBlockingInputs = true;
}

void Engine::unblock_inputs()
{
    isBlockingInputs = false;
}
