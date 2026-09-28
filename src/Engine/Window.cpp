#include "Window.h"
#include "spdlog/spdlog.h"
#include <stdexcept>
#include "events/Event.h"
#include "Engine/Engine.h"

Window::Window(const char* startTitle, uint32_t startWidth, uint32_t startHeight, bool startFullscreen)
	: title(startTitle),
	  height(startHeight),
	  width(startWidth),
	  isFullscreen(startFullscreen)
{
	fullscreenHeight = 0;
	fullscreenWidth = 0;
	glfwWindow = nullptr;
}

void Window::Init()
{
    glfwSetErrorCallback([](int error, const char* description) 
        {
        spdlog::error("GLFW Error {}: {}", error, description);
        });

#ifdef SW_DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

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

#ifdef SW_DEBUG

    int flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); // Makes sure errors print immediately
        glDebugMessageCallback([](GLenum source, GLenum type, unsigned int id, GLenum severity,
            GLsizei length, const char* message, const void* userParam)
            {
                // Ignore non-significant error/warning codes
                if (id == 131185 || id == 131218 || id == 131204 || id == 1282 || id == 131169) return;

                std::string sourceStr, typeStr, severityStr;

                switch (severity) {
                case GL_DEBUG_SEVERITY_HIGH:         severityStr = "HIGH"; break;
                case GL_DEBUG_SEVERITY_MEDIUM:       severityStr = "MEDIUM"; break;
                case GL_DEBUG_SEVERITY_LOW:          severityStr = "LOW"; break;
                case GL_DEBUG_SEVERITY_NOTIFICATION: severityStr = "NOTIFICATION"; break;
                }

                if (severity == GL_DEBUG_SEVERITY_HIGH || severity == GL_DEBUG_SEVERITY_MEDIUM)
                    spdlog::error("OpenGL Error [{}] ({}): {}", severityStr, id, message);
                else
                    spdlog::warn("OpenGL Error [{}] ({}): {}", severityStr, id, message);
            }, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
        spdlog::info("OpenGL Debug Output Enabled.");
    }
#endif

	spdlog::info("Glad initialised on window: {0}", title);
	
	SetEventCallbacks();

    if (glfwRawMouseMotionSupported()) 
    {
        glfwSetInputMode(glfwWindow, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }
    EnableCursor();
    glfwGetCursorPos(glfwWindow, &m_LastX, &m_LastY);
}

void Window::Destroy()
{
    glfwDestroyWindow(glfwWindow);
}

void Window::SetEventCallbacks()
{
    glfwSetWindowSizeCallback(glfwWindow, [](GLFWwindow* window, int width, int height)
        {
            int frameBufferWidth;
            int frameBufferHeight;
            glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);

            Engine::Events().FireEvent<WindowResizeEvent>(width, height, frameBufferWidth, frameBufferHeight);
        });

	glfwSetKeyCallback(glfwWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
            key = GLFWHelpers::glfw_key_to_SW_key(key);
            mods = GLFWHelpers::glfw_mods_to_SW_mods(mods);

			switch (action)
			{
			case GLFW_PRESS:
			{
				Engine::Events().FireEvent<KeyPressEvent>(key, scancode, mods);
				break;
			}
            case GLFW_REPEAT:
            {
                Engine::Events().FireEvent<KeyRepeatEvent>(key, scancode, mods);
                break;
            }
			case GLFW_RELEASE:
				Engine::Events().FireEvent<KeyReleaseEvent>(key, scancode, mods);
				break;
			}
		});

	glfwSetCharCallback(glfwWindow, [](GLFWwindow* window, unsigned int codePoint)
		{
			Engine::Events().FireEvent<CharEvent>(codePoint);
		});

	glfwSetCursorPosCallback(glfwWindow, [](GLFWwindow* window, double xpos, double ypos)
		{
			Engine::Events().FireEvent<MouseMoveEvent>(xpos, ypos);
		});

	glfwSetMouseButtonCallback(glfwWindow, [](GLFWwindow* window, int button, int action, int mods)
		{
            button = GLFWHelpers::glfw_mouse_button_to_SW_mouse_button(button);
            mods = GLFWHelpers::glfw_mods_to_SW_mods(mods);

			if (action == GLFW_PRESS)
			{
				Engine::Events().FireEvent<MousePressEvent>(button, mods);
				return;
			}
			if (action == GLFW_RELEASE)
			{
				Engine::Events().FireEvent<MouseReleaseEvent>(button, mods);
				return;
			}
		});

	glfwSetScrollCallback(glfwWindow, [](GLFWwindow* window, double x_offset, double y_offset)
		{
			Engine::Events().FireEvent<MouseWheelEvent>(x_offset, y_offset);
		});
}

void Window::Update()
{
	glfwSwapBuffers(glfwWindow);
	glfwPollEvents();
}

void Window::SetOpenGLContext()
{
	glfwMakeContextCurrent(glfwWindow);
}

void Window::MakeFullscreen()
{
	if (!glfwWindow)
	{
		spdlog::error("Attempted to set fullscreen on uninitialised window");
		return;
	}

	glfwSetWindowMonitor(glfwWindow, glfwGetPrimaryMonitor(), 0, 0, fullscreenWidth, fullscreenHeight, 0);
	isFullscreen = true;
}

void Window::MakeWindowed()
{
	if (!glfwWindow)
	{
		spdlog::error("Attempted to set windowed on uninitialised window");
		return;
	}

	glfwSetWindowMonitor(glfwWindow, NULL, 100, 100, width, height, 0);
	isFullscreen = false;
}

void Window::ToggleFullscreen()
{
	if (isFullscreen)
		MakeWindowed();
	else
		MakeFullscreen();
}

bool Window::GetKey(int keycode)
{
	return glfwGetKey(glfwWindow, keycode) == GLFW_PRESS;
}

bool Window::GetMouseButton(int button)
{
	return glfwGetMouseButton(glfwWindow, button);
}

void Window::GetMouseDelta(double* p_xpos, double* p_ypos)
{
    double xpos, ypos;
    glfwGetCursorPos(glfwWindow, &xpos, &ypos);

    *p_xpos = (float)(xpos - m_LastX);
    *p_ypos = (float)(ypos - m_LastY);

    m_LastX = xpos;
    m_LastY = ypos;
}

void Window::EnableCursor()
{
    glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Window::DisableCursor()
{
    glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Window::GetFrameBufferSize(int* width, int* height)
{
    glfwGetFramebufferSize(glfwWindow, width, height);
}

void Window::GetContentScale(float* x, float* y)
{
    glfwGetWindowContentScale(glfwWindow, x, y);
}

namespace GLFWHelpers {

    int glfw_key_to_SW_key(int glfw_key) {
        switch (glfw_key) {
        case GLFW_KEY_SPACE:         return SW_KEY_SPACE;
        case GLFW_KEY_APOSTROPHE:    return SW_KEY_APOSTROPHE;
        case GLFW_KEY_COMMA:         return SW_KEY_COMMA;
        case GLFW_KEY_MINUS:         return SW_KEY_MINUS;
        case GLFW_KEY_PERIOD:        return SW_KEY_PERIOD;
        case GLFW_KEY_SLASH:         return SW_KEY_SLASH;
        case GLFW_KEY_0:             return SW_KEY_0;
        case GLFW_KEY_1:             return SW_KEY_1;
        case GLFW_KEY_2:             return SW_KEY_2;
        case GLFW_KEY_3:             return SW_KEY_3;
        case GLFW_KEY_4:             return SW_KEY_4;
        case GLFW_KEY_5:             return SW_KEY_5;
        case GLFW_KEY_6:             return SW_KEY_6;
        case GLFW_KEY_7:             return SW_KEY_7;
        case GLFW_KEY_8:             return SW_KEY_8;
        case GLFW_KEY_9:             return SW_KEY_9;
        case GLFW_KEY_SEMICOLON:     return SW_KEY_SEMICOLON;
        case GLFW_KEY_EQUAL:         return SW_KEY_EQUAL;
        case GLFW_KEY_A:             return SW_KEY_A;
        case GLFW_KEY_B:             return SW_KEY_B;
        case GLFW_KEY_C:             return SW_KEY_C;
        case GLFW_KEY_D:             return SW_KEY_D;
        case GLFW_KEY_E:             return SW_KEY_E;
        case GLFW_KEY_F:             return SW_KEY_F;
        case GLFW_KEY_G:             return SW_KEY_G;
        case GLFW_KEY_H:             return SW_KEY_H;
        case GLFW_KEY_I:             return SW_KEY_I;
        case GLFW_KEY_J:             return SW_KEY_J;
        case GLFW_KEY_K:             return SW_KEY_K;
        case GLFW_KEY_L:             return SW_KEY_L;
        case GLFW_KEY_M:             return SW_KEY_M;
        case GLFW_KEY_N:             return SW_KEY_N;
        case GLFW_KEY_O:             return SW_KEY_O;
        case GLFW_KEY_P:             return SW_KEY_P;
        case GLFW_KEY_Q:             return SW_KEY_Q;
        case GLFW_KEY_R:             return SW_KEY_R;
        case GLFW_KEY_S:             return SW_KEY_S;
        case GLFW_KEY_T:             return SW_KEY_T;
        case GLFW_KEY_U:             return SW_KEY_U;
        case GLFW_KEY_V:             return SW_KEY_V;
        case GLFW_KEY_W:             return SW_KEY_W;
        case GLFW_KEY_X:             return SW_KEY_X;
        case GLFW_KEY_Y:             return SW_KEY_Y;
        case GLFW_KEY_Z:             return SW_KEY_Z;
        case GLFW_KEY_LEFT_BRACKET:  return SW_KEY_LEFT_BRACKET;
        case GLFW_KEY_BACKSLASH:     return SW_KEY_BACKSLASH;
        case GLFW_KEY_RIGHT_BRACKET: return SW_KEY_RIGHT_BRACKET;
        case GLFW_KEY_GRAVE_ACCENT:  return SW_KEY_GRAVE_ACCENT;
        case GLFW_KEY_ESCAPE:        return SW_KEY_ESCAPE;
        case GLFW_KEY_ENTER:         return SW_KEY_ENTER;
        case GLFW_KEY_TAB:           return SW_KEY_TAB;
        case GLFW_KEY_BACKSPACE:     return SW_KEY_BACKSPACE;
        case GLFW_KEY_INSERT:        return SW_KEY_INSERT;
        case GLFW_KEY_DELETE:        return SW_KEY_DELETE;
        case GLFW_KEY_RIGHT:         return SW_KEY_RIGHT;
        case GLFW_KEY_LEFT:          return SW_KEY_LEFT;
        case GLFW_KEY_DOWN:          return SW_KEY_DOWN;
        case GLFW_KEY_UP:            return SW_KEY_UP;
        case GLFW_KEY_PAGE_UP:       return SW_KEY_PAGE_UP;
        case GLFW_KEY_PAGE_DOWN:     return SW_KEY_PAGE_DOWN;
        case GLFW_KEY_HOME:          return SW_KEY_HOME;
        case GLFW_KEY_END:           return SW_KEY_END;
        case GLFW_KEY_CAPS_LOCK:     return SW_KEY_CAPS_LOCK;
        case GLFW_KEY_SCROLL_LOCK:   return SW_KEY_SCROLL_LOCK;
        case GLFW_KEY_NUM_LOCK:      return SW_KEY_NUM_LOCK;
        case GLFW_KEY_PRINT_SCREEN:  return SW_KEY_PRINT_SCREEN;
        case GLFW_KEY_PAUSE:         return SW_KEY_PAUSE;
        case GLFW_KEY_F1:            return SW_KEY_F1;
        case GLFW_KEY_F2:            return SW_KEY_F2;
        case GLFW_KEY_F3:            return SW_KEY_F3;
        case GLFW_KEY_F4:            return SW_KEY_F4;
        case GLFW_KEY_F5:            return SW_KEY_F5;
        case GLFW_KEY_F6:            return SW_KEY_F6;
        case GLFW_KEY_F7:            return SW_KEY_F7;
        case GLFW_KEY_F8:            return SW_KEY_F8;
        case GLFW_KEY_F9:            return SW_KEY_F9;
        case GLFW_KEY_F10:           return SW_KEY_F10;
        case GLFW_KEY_F11:           return SW_KEY_F11;
        case GLFW_KEY_F12:           return SW_KEY_F12;
        case GLFW_KEY_KP_0:          return SW_KEY_KP_0;
        case GLFW_KEY_KP_1:          return SW_KEY_KP_1;
        case GLFW_KEY_KP_2:          return SW_KEY_KP_2;
        case GLFW_KEY_KP_3:          return SW_KEY_KP_3;
        case GLFW_KEY_KP_4:          return SW_KEY_KP_4;
        case GLFW_KEY_KP_5:          return SW_KEY_KP_5;
        case GLFW_KEY_KP_6:          return SW_KEY_KP_6;
        case GLFW_KEY_KP_7:          return SW_KEY_KP_7;
        case GLFW_KEY_KP_8:          return SW_KEY_KP_8;
        case GLFW_KEY_KP_9:          return SW_KEY_KP_9;
        case GLFW_KEY_KP_DECIMAL:    return SW_KEY_KP_DECIMAL;
        case GLFW_KEY_KP_DIVIDE:     return SW_KEY_KP_DIVIDE;
        case GLFW_KEY_KP_MULTIPLY:   return SW_KEY_KP_MULTIPLY;
        case GLFW_KEY_KP_SUBTRACT:   return SW_KEY_KP_SUBTRACT;
        case GLFW_KEY_KP_ADD:        return SW_KEY_KP_ADD;
        case GLFW_KEY_KP_ENTER:      return SW_KEY_KP_ENTER;
        case GLFW_KEY_KP_EQUAL:      return SW_KEY_KP_EQUAL;
        case GLFW_KEY_LEFT_SHIFT:    return SW_KEY_LEFT_SHIFT;
        case GLFW_KEY_LEFT_CONTROL:  return SW_KEY_LEFT_CONTROL;
        case GLFW_KEY_LEFT_ALT:      return SW_KEY_LEFT_ALT;
        case GLFW_KEY_LEFT_SUPER:    return SW_KEY_LEFT_SUPER;
        case GLFW_KEY_RIGHT_SHIFT:   return SW_KEY_RIGHT_SHIFT;
        case GLFW_KEY_RIGHT_CONTROL: return SW_KEY_RIGHT_CONTROL;
        case GLFW_KEY_RIGHT_ALT:     return SW_KEY_RIGHT_ALT;
        case GLFW_KEY_RIGHT_SUPER:   return SW_KEY_RIGHT_SUPER;
        case GLFW_KEY_MENU:          return SW_KEY_MENU;
        default:                     return SW_KEY_UNKNOWN;
        }
    }

    int glfw_mouse_button_to_SW_mouse_button(int glfw_button) {
        switch (glfw_button) {
        case GLFW_MOUSE_BUTTON_LEFT:   return SW_MOUSE_BUTTON_LEFT;
        case GLFW_MOUSE_BUTTON_RIGHT:  return SW_MOUSE_BUTTON_RIGHT;
        case GLFW_MOUSE_BUTTON_MIDDLE: return SW_MOUSE_BUTTON_MIDDLE;
        case GLFW_MOUSE_BUTTON_4:      return SW_MOUSE_BUTTON_4;
        case GLFW_MOUSE_BUTTON_5:      return SW_MOUSE_BUTTON_5;
        case GLFW_MOUSE_BUTTON_6:      return SW_MOUSE_BUTTON_6;
        case GLFW_MOUSE_BUTTON_7:      return SW_MOUSE_BUTTON_7;
        case GLFW_MOUSE_BUTTON_8:      return SW_MOUSE_BUTTON_8;
        default:                       return -1;
        }
    }

    int glfw_mods_to_SW_mods(int glfw_mods) {
        int SW_mods = 0;
        if (glfw_mods & GLFW_MOD_SHIFT)     SW_mods |= SW_MOD_SHIFT;
        if (glfw_mods & GLFW_MOD_CONTROL)   SW_mods |= SW_MOD_CONTROL;
        if (glfw_mods & GLFW_MOD_ALT)       SW_mods |= SW_MOD_ALT;
        if (glfw_mods & GLFW_MOD_SUPER)     SW_mods |= SW_MOD_SUPER;
        if (glfw_mods & GLFW_MOD_CAPS_LOCK) SW_mods |= SW_MOD_CAPS_LOCK;
        if (glfw_mods & GLFW_MOD_NUM_LOCK)  SW_mods |= SW_MOD_NUM_LOCK;
        return SW_mods;
    }

    int glfw_joystick_to_SW_joystick(int glfw_joystick) {
        switch (glfw_joystick) {
        case GLFW_JOYSTICK_1:  return SW_JOYSTICK_1;
        case GLFW_JOYSTICK_2:  return SW_JOYSTICK_2;
        case GLFW_JOYSTICK_3:  return SW_JOYSTICK_3;
        case GLFW_JOYSTICK_4:  return SW_JOYSTICK_4;
        case GLFW_JOYSTICK_5:  return SW_JOYSTICK_5;
        case GLFW_JOYSTICK_6:  return SW_JOYSTICK_6;
        case GLFW_JOYSTICK_7:  return SW_JOYSTICK_7;
        case GLFW_JOYSTICK_8:  return SW_JOYSTICK_8;
        case GLFW_JOYSTICK_9:  return SW_JOYSTICK_9;
        case GLFW_JOYSTICK_10: return SW_JOYSTICK_10;
        case GLFW_JOYSTICK_11: return SW_JOYSTICK_11;
        case GLFW_JOYSTICK_12: return SW_JOYSTICK_12;
        case GLFW_JOYSTICK_13: return SW_JOYSTICK_13;
        case GLFW_JOYSTICK_14: return SW_JOYSTICK_14;
        case GLFW_JOYSTICK_15: return SW_JOYSTICK_15;
        case GLFW_JOYSTICK_16: return SW_JOYSTICK_16;
        default:               return -1;
        }
    }

    int glfw_gamepad_button_to_SW_gamepad_button(int glfw_button) {
        switch (glfw_button) {
        case GLFW_GAMEPAD_BUTTON_A:            return SW_GAMEPAD_BUTTON_A;
        case GLFW_GAMEPAD_BUTTON_B:            return SW_GAMEPAD_BUTTON_B;
        case GLFW_GAMEPAD_BUTTON_X:            return SW_GAMEPAD_BUTTON_X;
        case GLFW_GAMEPAD_BUTTON_Y:            return SW_GAMEPAD_BUTTON_Y;
        case GLFW_GAMEPAD_BUTTON_LEFT_BUMPER:  return SW_GAMEPAD_BUTTON_LEFT_BUMPER;
        case GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER: return SW_GAMEPAD_BUTTON_RIGHT_BUMPER;
        case GLFW_GAMEPAD_BUTTON_BACK:         return SW_GAMEPAD_BUTTON_BACK;
        case GLFW_GAMEPAD_BUTTON_START:        return SW_GAMEPAD_BUTTON_START;
        case GLFW_GAMEPAD_BUTTON_GUIDE:        return SW_GAMEPAD_BUTTON_GUIDE;
        case GLFW_GAMEPAD_BUTTON_LEFT_THUMB:   return SW_GAMEPAD_BUTTON_LEFT_THUMB;
        case GLFW_GAMEPAD_BUTTON_RIGHT_THUMB:  return SW_GAMEPAD_BUTTON_RIGHT_THUMB;
        case GLFW_GAMEPAD_BUTTON_DPAD_UP:      return SW_GAMEPAD_BUTTON_DPAD_UP;
        case GLFW_GAMEPAD_BUTTON_DPAD_RIGHT:   return SW_GAMEPAD_BUTTON_DPAD_RIGHT;
        case GLFW_GAMEPAD_BUTTON_DPAD_DOWN:    return SW_GAMEPAD_BUTTON_DPAD_DOWN;
        case GLFW_GAMEPAD_BUTTON_DPAD_LEFT:    return SW_GAMEPAD_BUTTON_DPAD_LEFT;
        default:                               return -1;
        }
    }

    int glfw_gamepad_axis_to_SW_gamepad_axis(int glfw_axis) {
        switch (glfw_axis) {
        case GLFW_GAMEPAD_AXIS_LEFT_X:        return SW_GAMEPAD_AXIS_LEFT_X;
        case GLFW_GAMEPAD_AXIS_LEFT_Y:        return SW_GAMEPAD_AXIS_LEFT_Y;
        case GLFW_GAMEPAD_AXIS_RIGHT_X:       return SW_GAMEPAD_AXIS_RIGHT_X;
        case GLFW_GAMEPAD_AXIS_RIGHT_Y:       return SW_GAMEPAD_AXIS_RIGHT_Y;
        case GLFW_GAMEPAD_AXIS_LEFT_TRIGGER:  return SW_GAMEPAD_AXIS_LEFT_TRIGGER;
        case GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER: return SW_GAMEPAD_AXIS_RIGHT_TRIGGER;
        default:                              return -1;
        }
    }
}