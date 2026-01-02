#include "InputManager.h"
#include "spdlog/spdlog.h"
#include "Engine/Engine.h"

InputManager::InputManager(Window* w)
    : mWindow(w)
{
}

void InputManager::init()
{
    spdlog::info("Intialising input manager");
    Engine::get_engine()->get_event_system().subscribe<ImGuiReleaseControlEvent, InputManager, &InputManager::on_imgui_release_control>(Layer::ENGINE, this);
    Engine::get_engine()->get_event_system().subscribe<ImGuiTakeControlEvent, InputManager, &InputManager::on_imgui_take_control>(Layer::ENGINE, this);
}

void InputManager::set_window(Window* w)
{
    mWindow = w;
}

bool InputManager::get_key(int keycode)
{
    if (!m_IsAcceptingInput) return false;

    return mWindow->get_key(keycode);
}

bool InputManager::get_mouse_button(int button)
{
    if (!m_IsAcceptingInput) return false;

    return mWindow->get_mouse_button(button);
}

std::pair<double, double> InputManager::get_mouse()
{
    if (!m_IsAcceptingInput) return std::pair<double, double>(0.0, 0.0);

    double xpos, ypos;
    mWindow->get_mouse_delta(&xpos, &ypos);
    return std::pair<double, double>(xpos, ypos);
}

double InputManager::get_mouseX()
{
    if (!m_IsAcceptingInput) return 0.0;

    double xpos, ypos;
    mWindow->get_mouse_delta(&xpos, &ypos);
    return xpos;
}

double InputManager::get_mouseY()
{
    if (!m_IsAcceptingInput) return 0.0;

    double xpos, ypos;
    mWindow->get_mouse_delta(&xpos, &ypos);
    return ypos;
}

void InputManager::on_imgui_release_control(ImGuiReleaseControlEvent& e)
{
    m_IsAcceptingInput = true;
}

void InputManager::on_imgui_take_control(ImGuiTakeControlEvent& e)
{
    m_IsAcceptingInput = false;
}
