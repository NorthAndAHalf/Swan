#include "InputManager.h"
#include "spdlog/spdlog.h"
#include "Engine/Engine.h"

InputManager::InputManager(Window* w)
    : mWindow(w)
{
}

void InputManager::Init()
{
    spdlog::info("Intialising input manager");
    Engine::Events().Subscribe<ImGuiReleaseControlEvent, InputManager, &InputManager::OnImguiReleaseControl>(Layer::ENGINE, this);
    Engine::Events().Subscribe<ImGuiTakeControlEvent, InputManager, &InputManager::OnImguiTakeControl>(Layer::ENGINE, this);
}

void InputManager::SetWindow(Window* w)
{
    mWindow = w;
}

bool InputManager::GetKey(int keycode)
{
    if (!m_IsAcceptingInput) return false;

    return mWindow->GetKey(keycode);
}

bool InputManager::GetMouseButton(int button)
{
    if (!m_IsAcceptingInput) return false;

    return mWindow->GetMouseButton(button);
}

std::pair<double, double> InputManager::GetMouse()
{
    if (!m_IsAcceptingInput) return std::pair<double, double>(0.0, 0.0);

    double xpos, ypos;
    mWindow->GetMouseDelta(&xpos, &ypos);
    return std::pair<double, double>(xpos, ypos);
}

double InputManager::GetMouseX()
{
    if (!m_IsAcceptingInput) return 0.0;

    double xpos, ypos;
    mWindow->GetMouseDelta(&xpos, &ypos);
    return xpos;
}

double InputManager::GetMouseY()
{
    if (!m_IsAcceptingInput) return 0.0;

    double xpos, ypos;
    mWindow->GetMouseDelta(&xpos, &ypos);
    return ypos;
}

void InputManager::OnImguiReleaseControl(ImGuiReleaseControlEvent& e)
{
    m_IsAcceptingInput = true;
}

void InputManager::OnImguiTakeControl(ImGuiTakeControlEvent& e)
{
    m_IsAcceptingInput = false;
}
