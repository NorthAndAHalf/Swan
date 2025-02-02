#include "glad/glad.h"
#include "Window.h"
#include "spdlog/spdlog.h"
#include <stdexcept>t

Window::Window(const char* startTitle, uint32_t startHeight, uint32_t startWidth, bool startFullscreen)
	: title(startTitle),
	  height(startHeight),
	  width(startWidth),
	  isFullscreen(startFullscreen)
{
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	fullscreenHeight = mode->height;
	fullscreenWidth = mode->width;
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
	
	glfwMakeContextCurrent(glfwWindow);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		glfwTerminate();
		throw std::runtime_error("Failed to initialise glad");
	}
	
	glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	glClearColor(255.0f, 0.0f, 255.0f, 1.0f);
}

void Window::update()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glfwSwapBuffers(glfwWindow);
	glfwPollEvents();
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
