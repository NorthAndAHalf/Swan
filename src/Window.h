#pragma once
#include <stdint.h>
#include "GLFW/glfw3.h"
#include "Events/EventDispatcher.h"

class Window
{
public:
	Window(const char* startTitle, uint32_t startHeight, uint32_t startWidth, bool startFullscreen);

	void init();
	void update();

	void init_imgui();

	void set_title(const char* newTitle) { title = newTitle; }
	const char* get_title() { return title; }

	void set_height(uint32_t newHeight) { 
		height = newHeight; 
	}

	uint32_t get_height() { 
		return (isFullscreen) ? fullscreenHeight : height;
	}

	void set_width(uint32_t newWidth) { 
		width = newWidth; 
	}

	uint32_t get_width() { 
		return (isFullscreen) ? fullscreenWidth : width; 
	}

	void set_fullscreen_height(uint32_t height) { fullscreenHeight = height; }
	void set_fullscreen_width(uint32_t width) { fullscreenWidth = width;  }

	bool is_fullscreen() { return isFullscreen; }
	void make_fullscreen();
	void make_windowed();
	void toggle_fullscreen();

	bool window_should_close() { return glfwWindowShouldClose(glfwWindow); }

	// Input Polling
	bool get_key(int keycode);
	bool get_mouse_button(int button);
	void get_cursor(double* xpos, double* ypos);

private:
	const char* title;
	uint32_t height;
	uint32_t width;

	uint32_t fullscreenHeight;
	uint32_t fullscreenWidth;

	bool isFullscreen;

	GLFWwindow* glfwWindow;

	void set_event_callbacks();
};
