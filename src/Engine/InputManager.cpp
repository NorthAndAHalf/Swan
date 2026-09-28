#include "InputManager.h"
#include "Engine/Engine.h"
#include "../Core.h"

#define DEBUG_TOGGLE_KEY SW_KEY_ESCAPE

InputManager::InputManager(Window* w)
{
    // Only happens once on startup, or on a rare input system change, so a heap allocation is ok here
    // In future, I'll have a core systems heap that this will be allocated on to improve cache coherency with other central systems
    m_InputSource = new WindowInputSource(w);

    Engine::Events().Subscribe<KeyPressEvent>([this](const KeyPressEvent& e)
        {
            if (e.keycode == DEBUG_TOGGLE_KEY)
            {
                m_IsDebugMode = !m_IsDebugMode;
                return;
            }

            uint32_t typeId = TypeIdentifier::GetId<KeyPressEvent>();
            auto listeners = m_IsDebugMode ? m_DebugListeners[typeId] : m_Listeners[typeId];
            for (InputListener* l : listeners)
                l->OnKeyPress(e.keycode, e.scancode, e.mods);
        });

    Engine::Events().Subscribe<KeyRepeatEvent>([this](const KeyRepeatEvent& e)
        {
            uint32_t typeId = TypeIdentifier::GetId<KeyRepeatEvent>();
            auto listeners = m_IsDebugMode ? m_DebugListeners[typeId] : m_Listeners[typeId];
            for (InputListener* l : listeners)
                l->OnKeyRepeat(e.keycode, e.scancode, e.mods);
        });

    Engine::Events().Subscribe<KeyReleaseEvent>([this](const KeyReleaseEvent& e)
        {
            uint32_t typeId = TypeIdentifier::GetId<KeyReleaseEvent>();
            auto listeners = m_IsDebugMode ? m_DebugListeners[typeId] : m_Listeners[typeId];
            for (InputListener* l : listeners)
                l->OnKeyRelease(e.keycode, e.scancode, e.mods);
        });

    Engine::Events().Subscribe<CharEvent>([this](const CharEvent& e)
        {
            uint32_t typeId = TypeIdentifier::GetId<CharEvent>();
            auto listeners = m_IsDebugMode ? m_DebugListeners[typeId] : m_Listeners[typeId];
            for (InputListener* l : listeners)
                l->OnCharInput(e.codePoint);
        });

    Engine::Events().Subscribe<MouseMoveEvent>([this](const MouseMoveEvent& e)
        {
            uint32_t typeId = TypeIdentifier::GetId<MouseMoveEvent>();
            auto listeners = m_IsDebugMode ? m_DebugListeners[typeId] : m_Listeners[typeId];
            for (InputListener* l : listeners)
                l->OnMouseMove(e.xpos, e.ypos);
        });

    Engine::Events().Subscribe<MousePressEvent>([this](const MousePressEvent& e)
        {
            uint32_t typeId = TypeIdentifier::GetId<MousePressEvent>();
            auto listeners = m_IsDebugMode ? m_DebugListeners[typeId] : m_Listeners[typeId];
            for (InputListener* l : listeners)
                l->OnMousePress(e.button, e.mods);
        });

    Engine::Events().Subscribe<MouseReleaseEvent>([this](const MouseReleaseEvent& e)
        {
            uint32_t typeId = TypeIdentifier::GetId<MouseReleaseEvent>();
            auto listeners = m_IsDebugMode ? m_DebugListeners[typeId] : m_Listeners[typeId];
            for (InputListener* l : listeners)
                l->OnMouseRelease(e.button, e.mods);
        });

    Engine::Events().Subscribe<MouseWheelEvent>([this](const MouseWheelEvent& e)
        {
            uint32_t typeId = TypeIdentifier::GetId<MouseWheelEvent>();
            auto listeners = m_IsDebugMode ? m_DebugListeners[typeId] : m_Listeners[typeId];
            for (InputListener* l : listeners)
                l->OnMouseWheel(e.x_offset, e.y_offset);
        });


}

InputManager::~InputManager()
{
    delete m_InputSource;
}

void InputManager::Update()
{
    m_InputSource->GetMouse(&m_MouseX, &m_MouseY);
}

bool InputManager::GetKey(int keycode)
{
    return m_InputSource->GetKey(keycode);
}

bool InputManager::GetMouseButton(int button)
{
    return m_InputSource->GetMouseButton(button);
}

std::pair<double, double> InputManager::GetMouse()
{
    return std::pair<double, double>(m_MouseX, m_MouseY);
}

double InputManager::GetMouseX()
{
    return m_MouseX;
}

double InputManager::GetMouseY()
{
    return m_MouseY;
}

WindowInputSource::WindowInputSource(Window* window)
    : m_Window(window)
{
}

bool WindowInputSource::GetKey(int keycode)
{
    return m_Window->GetKey(keycode);
}

bool WindowInputSource::GetMouseButton(int button)
{
    return m_Window->GetMouseButton(button);
}

void WindowInputSource::GetMouse(double* xpos, double* ypos)
{
    m_Window->GetMouseDelta(xpos, ypos);
}