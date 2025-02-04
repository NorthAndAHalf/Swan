#include "glad/glad.h"
#include "Window.h"
#include "spdlog/spdlog.h"
#include <stdexcept>
#include "events/Event.h"
#include "Engine.h"

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

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
			switch (action)
			{
			case GLFW_PRESS:
			{
				Engine::get_engine()->get_event_dispatcher().dispatch_key_press(KeyPressEvent(key, scancode, mods));
				break;
			}
			case GLFW_RELEASE:
				Engine::get_engine()->get_event_dispatcher().dispatch_key_release(KeyReleaseEvent(key, scancode, mods));
				break;
			}
		});

	glfwSetCursorPosCallback(glfwWindow, [](GLFWwindow* window, double xpos, double ypos)
		{
			Engine::get_engine()->get_event_dispatcher().dispatch_mouse_move(MouseMoveEvent(xpos, ypos));
		});

	glfwSetMouseButtonCallback(glfwWindow, [](GLFWwindow* window, int button, int action, int mods)
		{
			if (action == GLFW_PRESS)
			{
				Engine::get_engine()->get_event_dispatcher().dispatch_mouse_press(MousePressEvent(button, mods));
				return;
			}
			if (action == GLFW_RELEASE)
			{
				Engine::get_engine()->get_event_dispatcher().dispatch_mouse_release(MouseReleaseEvent(button, mods));
				return;
			}
		});
}

void Window::update()
{
	glfwSwapBuffers(glfwWindow);
	glfwPollEvents();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::init_imgui()
{
	ImGui_ImplGlfw_InitForOpenGL(glfwWindow, true);
	ImGui_ImplOpenGL3_Init();
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
