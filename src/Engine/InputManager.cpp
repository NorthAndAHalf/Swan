#include "InputManager.h"

InputManager::InputManager(Window* w)
    : mWindow(w), isBlockingInputs(false)
{
}

void InputManager::set_window(Window* w)
{
    mWindow = w;
}

bool InputManager::get_key(int keycode)
{
    if (isBlockingInputs) return false;
    return mWindow->get_key(keycode);
}

bool InputManager::get_mouse_button(int button)
{
    if (isBlockingInputs) return false;
    return mWindow->get_mouse_button(button);
}

std::pair<double, double> InputManager::get_cursor()
{
    double xpos, ypos;
    mWindow->get_cursor(&xpos, &ypos);
    return std::pair<double, double>(xpos, ypos);
}

double InputManager::get_cursorX()
{
    double xpos, ypos;
    mWindow->get_cursor(&xpos, &ypos);
    return xpos;
}

double InputManager::get_cursorY()
{
    double xpos, ypos;
    mWindow->get_cursor(&xpos, &ypos);
    return ypos;
}

void InputManager::block_inputs()
{
    isBlockingInputs = true;
}

void InputManager::unblock_inputs()
{
    isBlockingInputs = false;
}
