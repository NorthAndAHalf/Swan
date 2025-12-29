#include "glad/glad.h"
#include "Window.h"
#include "spdlog/spdlog.h"
#include <stdexcept>
#include "events/Event.h"
#include "Engine/Engine.h"

Window::Window(const char* startTitle, uint32_t startHeight, uint32_t startWidth, bool startFullscreen)
	: title(startTitle),
	  height(startHeight),
	  width(startWidth),
	  isFullscreen(startFullscreen)
{
	fullscreenHeight = 0;
	fullscreenWidth = 0;
	glfwWindow = nullptr;
}

void Window::init()
{
	if (isFullscreen)
	{
		glfwWindow = glfwCreateWindow(fullscreenWidth, fullscreenHeight, title, glfwGetPrimaryMonitor(), NULL);
	}
	else
	{
		glfwWindow = glfwCreateWindow(width, height, title, NULL, NULL);
	}
	
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	fullscreenHeight = mode->height;
	fullscreenWidth = mode->width;

	glfwMakeContextCurrent(glfwWindow);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		glfwTerminate();
		throw std::runtime_error("Failed to initialise glad");
	}

	spdlog::info("Glad initialised on window: {0}", title);
	
	set_event_callbacks();

	glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	glClearColor(255.0f, 0.0f, 255.0f, 1.0f);
}

void Window::set_event_callbacks()
{
	glfwSetKeyCallback(glfwWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
            key = GLFWHelpers::glfw_key_to_sf_key(key);
            mods = GLFWHelpers::glfw_mods_to_sf_mods(mods);

			switch (action)
			{
			case GLFW_PRESS:
			{
				Engine::get_engine()->get_event_system().queue_event<KeyPressEvent>(key, scancode, mods);
				break;
			}
			case GLFW_RELEASE:
				Engine::get_engine()->get_event_system().queue_event<KeyReleaseEvent>(key, scancode, mods);
				break;
			}
		});

	glfwSetCharCallback(glfwWindow, [](GLFWwindow* window, unsigned int codePoint)
		{
			Engine::get_engine()->get_event_system().queue_event<CharEvent>(codePoint);
		});

	glfwSetCursorPosCallback(glfwWindow, [](GLFWwindow* window, double xpos, double ypos)
		{
			Engine::get_engine()->get_event_system().queue_event<MouseMoveEvent>(xpos, ypos);
		});

	glfwSetMouseButtonCallback(glfwWindow, [](GLFWwindow* window, int button, int action, int mods)
		{
            button = GLFWHelpers::glfw_mouse_button_to_sf_mouse_button(button);
            mods = GLFWHelpers::glfw_mods_to_sf_mods(mods);

			if (action == GLFW_PRESS)
			{
				Engine::get_engine()->get_event_system().queue_event<MousePressEvent>(button, mods);
				return;
			}
			if (action == GLFW_RELEASE)
			{
				Engine::get_engine()->get_event_system().queue_event<MouseReleaseEvent>(button, mods);
				return;
			}
		});

	glfwSetScrollCallback(glfwWindow, [](GLFWwindow* window, double x_offset, double y_offset)
		{
			Engine::get_engine()->get_event_system().queue_event<MouseWheelEvent>(x_offset, y_offset);
		});
}

void Window::update()
{
	glfwSwapBuffers(glfwWindow);
	glfwPollEvents();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::set_opengl_context()
{
	glfwMakeContextCurrent(glfwWindow);
}

void Window::make_fullscreen()
{
	if (!glfwWindow)
	{
		spdlog::error("Attempted to set fullscreen on uninitialised window");
		return;
	}

	glfwSetWindowMonitor(glfwWindow, glfwGetPrimaryMonitor(), 0, 0, fullscreenWidth, fullscreenHeight, 0);
	isFullscreen = true;
}

void Window::make_windowed()
{
	if (!glfwWindow)
	{
		spdlog::error("Attempted to set windowed on uninitialised window");
		return;
	}

	glfwSetWindowMonitor(glfwWindow, NULL, 100, 100, width, height, 0);
	isFullscreen = false;
}

void Window::toggle_fullscreen()
{
	if (isFullscreen)
		make_windowed();
	else
		make_fullscreen();
}

bool Window::get_key(int keycode)
{
	return glfwGetKey(glfwWindow, keycode) == GLFW_PRESS;
}

bool Window::get_mouse_button(int button)
{
	return glfwGetMouseButton(glfwWindow, button);
}

void Window::get_cursor(double* xpos, double* ypos)
{
	glfwGetCursorPos(glfwWindow, xpos, ypos);
}

namespace GLFWHelpers {

    int glfw_key_to_sf_key(int glfw_key) {
        switch (glfw_key) {
        case GLFW_KEY_SPACE:         return SF_KEY_SPACE;
        case GLFW_KEY_APOSTROPHE:    return SF_KEY_APOSTROPHE;
        case GLFW_KEY_COMMA:         return SF_KEY_COMMA;
        case GLFW_KEY_MINUS:         return SF_KEY_MINUS;
        case GLFW_KEY_PERIOD:        return SF_KEY_PERIOD;
        case GLFW_KEY_SLASH:         return SF_KEY_SLASH;
        case GLFW_KEY_0:             return SF_KEY_0;
        case GLFW_KEY_1:             return SF_KEY_1;
        case GLFW_KEY_2:             return SF_KEY_2;
        case GLFW_KEY_3:             return SF_KEY_3;
        case GLFW_KEY_4:             return SF_KEY_4;
        case GLFW_KEY_5:             return SF_KEY_5;
        case GLFW_KEY_6:             return SF_KEY_6;
        case GLFW_KEY_7:             return SF_KEY_7;
        case GLFW_KEY_8:             return SF_KEY_8;
        case GLFW_KEY_9:             return SF_KEY_9;
        case GLFW_KEY_SEMICOLON:     return SF_KEY_SEMICOLON;
        case GLFW_KEY_EQUAL:         return SF_KEY_EQUAL;
        case GLFW_KEY_A:             return SF_KEY_A;
        case GLFW_KEY_B:             return SF_KEY_B;
        case GLFW_KEY_C:             return SF_KEY_C;
        case GLFW_KEY_D:             return SF_KEY_D;
        case GLFW_KEY_E:             return SF_KEY_E;
        case GLFW_KEY_F:             return SF_KEY_F;
        case GLFW_KEY_G:             return SF_KEY_G;
        case GLFW_KEY_H:             return SF_KEY_H;
        case GLFW_KEY_I:             return SF_KEY_I;
        case GLFW_KEY_J:             return SF_KEY_J;
        case GLFW_KEY_K:             return SF_KEY_K;
        case GLFW_KEY_L:             return SF_KEY_L;
        case GLFW_KEY_M:             return SF_KEY_M;
        case GLFW_KEY_N:             return SF_KEY_N;
        case GLFW_KEY_O:             return SF_KEY_O;
        case GLFW_KEY_P:             return SF_KEY_P;
        case GLFW_KEY_Q:             return SF_KEY_Q;
        case GLFW_KEY_R:             return SF_KEY_R;
        case GLFW_KEY_S:             return SF_KEY_S;
        case GLFW_KEY_T:             return SF_KEY_T;
        case GLFW_KEY_U:             return SF_KEY_U;
        case GLFW_KEY_V:             return SF_KEY_V;
        case GLFW_KEY_W:             return SF_KEY_W;
        case GLFW_KEY_X:             return SF_KEY_X;
        case GLFW_KEY_Y:             return SF_KEY_Y;
        case GLFW_KEY_Z:             return SF_KEY_Z;
        case GLFW_KEY_LEFT_BRACKET:  return SF_KEY_LEFT_BRACKET;
        case GLFW_KEY_BACKSLASH:     return SF_KEY_BACKSLASH;
        case GLFW_KEY_RIGHT_BRACKET: return SF_KEY_RIGHT_BRACKET;
        case GLFW_KEY_GRAVE_ACCENT:  return SF_KEY_GRAVE_ACCENT;
        case GLFW_KEY_ESCAPE:        return SF_KEY_ESCAPE;
        case GLFW_KEY_ENTER:         return SF_KEY_ENTER;
        case GLFW_KEY_TAB:           return SF_KEY_TAB;
        case GLFW_KEY_BACKSPACE:     return SF_KEY_BACKSPACE;
        case GLFW_KEY_INSERT:        return SF_KEY_INSERT;
        case GLFW_KEY_DELETE:        return SF_KEY_DELETE;
        case GLFW_KEY_RIGHT:         return SF_KEY_RIGHT;
        case GLFW_KEY_LEFT:          return SF_KEY_LEFT;
        case GLFW_KEY_DOWN:          return SF_KEY_DOWN;
        case GLFW_KEY_UP:            return SF_KEY_UP;
        case GLFW_KEY_PAGE_UP:       return SF_KEY_PAGE_UP;
        case GLFW_KEY_PAGE_DOWN:     return SF_KEY_PAGE_DOWN;
        case GLFW_KEY_HOME:          return SF_KEY_HOME;
        case GLFW_KEY_END:           return SF_KEY_END;
        case GLFW_KEY_CAPS_LOCK:     return SF_KEY_CAPS_LOCK;
        case GLFW_KEY_SCROLL_LOCK:   return SF_KEY_SCROLL_LOCK;
        case GLFW_KEY_NUM_LOCK:      return SF_KEY_NUM_LOCK;
        case GLFW_KEY_PRINT_SCREEN:  return SF_KEY_PRINT_SCREEN;
        case GLFW_KEY_PAUSE:         return SF_KEY_PAUSE;
        case GLFW_KEY_F1:            return SF_KEY_F1;
        case GLFW_KEY_F2:            return SF_KEY_F2;
        case GLFW_KEY_F3:            return SF_KEY_F3;
        case GLFW_KEY_F4:            return SF_KEY_F4;
        case GLFW_KEY_F5:            return SF_KEY_F5;
        case GLFW_KEY_F6:            return SF_KEY_F6;
        case GLFW_KEY_F7:            return SF_KEY_F7;
        case GLFW_KEY_F8:            return SF_KEY_F8;
        case GLFW_KEY_F9:            return SF_KEY_F9;
        case GLFW_KEY_F10:           return SF_KEY_F10;
        case GLFW_KEY_F11:           return SF_KEY_F11;
        case GLFW_KEY_F12:           return SF_KEY_F12;
        case GLFW_KEY_KP_0:          return SF_KEY_KP_0;
        case GLFW_KEY_KP_1:          return SF_KEY_KP_1;
        case GLFW_KEY_KP_2:          return SF_KEY_KP_2;
        case GLFW_KEY_KP_3:          return SF_KEY_KP_3;
        case GLFW_KEY_KP_4:          return SF_KEY_KP_4;
        case GLFW_KEY_KP_5:          return SF_KEY_KP_5;
        case GLFW_KEY_KP_6:          return SF_KEY_KP_6;
        case GLFW_KEY_KP_7:          return SF_KEY_KP_7;
        case GLFW_KEY_KP_8:          return SF_KEY_KP_8;
        case GLFW_KEY_KP_9:          return SF_KEY_KP_9;
        case GLFW_KEY_KP_DECIMAL:    return SF_KEY_KP_DECIMAL;
        case GLFW_KEY_KP_DIVIDE:     return SF_KEY_KP_DIVIDE;
        case GLFW_KEY_KP_MULTIPLY:   return SF_KEY_KP_MULTIPLY;
        case GLFW_KEY_KP_SUBTRACT:   return SF_KEY_KP_SUBTRACT;
        case GLFW_KEY_KP_ADD:        return SF_KEY_KP_ADD;
        case GLFW_KEY_KP_ENTER:      return SF_KEY_KP_ENTER;
        case GLFW_KEY_KP_EQUAL:      return SF_KEY_KP_EQUAL;
        case GLFW_KEY_LEFT_SHIFT:    return SF_KEY_LEFT_SHIFT;
        case GLFW_KEY_LEFT_CONTROL:  return SF_KEY_LEFT_CONTROL;
        case GLFW_KEY_LEFT_ALT:      return SF_KEY_LEFT_ALT;
        case GLFW_KEY_LEFT_SUPER:    return SF_KEY_LEFT_SUPER;
        case GLFW_KEY_RIGHT_SHIFT:   return SF_KEY_RIGHT_SHIFT;
        case GLFW_KEY_RIGHT_CONTROL: return SF_KEY_RIGHT_CONTROL;
        case GLFW_KEY_RIGHT_ALT:     return SF_KEY_RIGHT_ALT;
        case GLFW_KEY_RIGHT_SUPER:   return SF_KEY_RIGHT_SUPER;
        case GLFW_KEY_MENU:          return SF_KEY_MENU;
        default:                     return SF_KEY_UNKNOWN;
        }
    }

    int glfw_mouse_button_to_sf_mouse_button(int glfw_button) {
        switch (glfw_button) {
        case GLFW_MOUSE_BUTTON_LEFT:   return SF_MOUSE_BUTTON_LEFT;
        case GLFW_MOUSE_BUTTON_RIGHT:  return SF_MOUSE_BUTTON_RIGHT;
        case GLFW_MOUSE_BUTTON_MIDDLE: return SF_MOUSE_BUTTON_MIDDLE;
        case GLFW_MOUSE_BUTTON_4:      return SF_MOUSE_BUTTON_4;
        case GLFW_MOUSE_BUTTON_5:      return SF_MOUSE_BUTTON_5;
        case GLFW_MOUSE_BUTTON_6:      return SF_MOUSE_BUTTON_6;
        case GLFW_MOUSE_BUTTON_7:      return SF_MOUSE_BUTTON_7;
        case GLFW_MOUSE_BUTTON_8:      return SF_MOUSE_BUTTON_8;
        default:                       return -1;
        }
    }

    int glfw_mods_to_sf_mods(int glfw_mods) {
        int sf_mods = 0;
        if (glfw_mods & GLFW_MOD_SHIFT)     sf_mods |= SF_MOD_SHIFT;
        if (glfw_mods & GLFW_MOD_CONTROL)   sf_mods |= SF_MOD_CONTROL;
        if (glfw_mods & GLFW_MOD_ALT)       sf_mods |= SF_MOD_ALT;
        if (glfw_mods & GLFW_MOD_SUPER)     sf_mods |= SF_MOD_SUPER;
        if (glfw_mods & GLFW_MOD_CAPS_LOCK) sf_mods |= SF_MOD_CAPS_LOCK;
        if (glfw_mods & GLFW_MOD_NUM_LOCK)  sf_mods |= SF_MOD_NUM_LOCK;
        return sf_mods;
    }

    int glfw_joystick_to_sf_joystick(int glfw_joystick) {
        switch (glfw_joystick) {
        case GLFW_JOYSTICK_1:  return SF_JOYSTICK_1;
        case GLFW_JOYSTICK_2:  return SF_JOYSTICK_2;
        case GLFW_JOYSTICK_3:  return SF_JOYSTICK_3;
        case GLFW_JOYSTICK_4:  return SF_JOYSTICK_4;
        case GLFW_JOYSTICK_5:  return SF_JOYSTICK_5;
        case GLFW_JOYSTICK_6:  return SF_JOYSTICK_6;
        case GLFW_JOYSTICK_7:  return SF_JOYSTICK_7;
        case GLFW_JOYSTICK_8:  return SF_JOYSTICK_8;
        case GLFW_JOYSTICK_9:  return SF_JOYSTICK_9;
        case GLFW_JOYSTICK_10: return SF_JOYSTICK_10;
        case GLFW_JOYSTICK_11: return SF_JOYSTICK_11;
        case GLFW_JOYSTICK_12: return SF_JOYSTICK_12;
        case GLFW_JOYSTICK_13: return SF_JOYSTICK_13;
        case GLFW_JOYSTICK_14: return SF_JOYSTICK_14;
        case GLFW_JOYSTICK_15: return SF_JOYSTICK_15;
        case GLFW_JOYSTICK_16: return SF_JOYSTICK_16;
        default:               return -1;
        }
    }

    int glfw_gamepad_button_to_sf_gamepad_button(int glfw_button) {
        switch (glfw_button) {
        case GLFW_GAMEPAD_BUTTON_A:            return SF_GAMEPAD_BUTTON_A;
        case GLFW_GAMEPAD_BUTTON_B:            return SF_GAMEPAD_BUTTON_B;
        case GLFW_GAMEPAD_BUTTON_X:            return SF_GAMEPAD_BUTTON_X;
        case GLFW_GAMEPAD_BUTTON_Y:            return SF_GAMEPAD_BUTTON_Y;
        case GLFW_GAMEPAD_BUTTON_LEFT_BUMPER:  return SF_GAMEPAD_BUTTON_LEFT_BUMPER;
        case GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER: return SF_GAMEPAD_BUTTON_RIGHT_BUMPER;
        case GLFW_GAMEPAD_BUTTON_BACK:         return SF_GAMEPAD_BUTTON_BACK;
        case GLFW_GAMEPAD_BUTTON_START:        return SF_GAMEPAD_BUTTON_START;
        case GLFW_GAMEPAD_BUTTON_GUIDE:        return SF_GAMEPAD_BUTTON_GUIDE;
        case GLFW_GAMEPAD_BUTTON_LEFT_THUMB:   return SF_GAMEPAD_BUTTON_LEFT_THUMB;
        case GLFW_GAMEPAD_BUTTON_RIGHT_THUMB:  return SF_GAMEPAD_BUTTON_RIGHT_THUMB;
        case GLFW_GAMEPAD_BUTTON_DPAD_UP:      return SF_GAMEPAD_BUTTON_DPAD_UP;
        case GLFW_GAMEPAD_BUTTON_DPAD_RIGHT:   return SF_GAMEPAD_BUTTON_DPAD_RIGHT;
        case GLFW_GAMEPAD_BUTTON_DPAD_DOWN:    return SF_GAMEPAD_BUTTON_DPAD_DOWN;
        case GLFW_GAMEPAD_BUTTON_DPAD_LEFT:    return SF_GAMEPAD_BUTTON_DPAD_LEFT;
        default:                               return -1;
        }
    }

    int glfw_gamepad_axis_to_sf_gamepad_axis(int glfw_axis) {
        switch (glfw_axis) {
        case GLFW_GAMEPAD_AXIS_LEFT_X:        return SF_GAMEPAD_AXIS_LEFT_X;
        case GLFW_GAMEPAD_AXIS_LEFT_Y:        return SF_GAMEPAD_AXIS_LEFT_Y;
        case GLFW_GAMEPAD_AXIS_RIGHT_X:       return SF_GAMEPAD_AXIS_RIGHT_X;
        case GLFW_GAMEPAD_AXIS_RIGHT_Y:       return SF_GAMEPAD_AXIS_RIGHT_Y;
        case GLFW_GAMEPAD_AXIS_LEFT_TRIGGER:  return SF_GAMEPAD_AXIS_LEFT_TRIGGER;
        case GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER: return SF_GAMEPAD_AXIS_RIGHT_TRIGGER;
        default:                              return -1;
        }
    }
}