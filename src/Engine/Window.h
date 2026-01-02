#pragma once
#include <stdint.h>
#include "GLFW/glfw3.h"
#include "Events/eventsystem.h"

class Window
{
public:
	Window(const char* startTitle, uint32_t startHeight, uint32_t startWidth, bool startFullscreen);

	void init();
	void destroy();
	void update();

	void set_opengl_context();

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

	void enable_cursor();
	void disable_cursor();

	void get_framebuffer_size(int* width, int* height);
	void get_content_scale(float* x, float* y);

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
	void get_mouse_delta(double* xpos, double* ypos);

private:
	const char* title;
	uint32_t height;
	uint32_t width;

	uint32_t fullscreenHeight;
	uint32_t fullscreenWidth;

	bool isFullscreen;

	GLFWwindow* glfwWindow;

	float m_LastX;
	float m_LastY;

	void set_event_callbacks();
};

namespace GLFWHelpers {
	// Key and Button Mappings
	int glfw_key_to_sf_key(int glfw_key);
	int glfw_mouse_button_to_sf_mouse_button(int glfw_button);
	int glfw_mods_to_sf_mods(int glfw_mods);
	int glfw_gamepad_button_to_sf_gamepad_button(int glfw_button);

	// Joystick and Axis Mappings
	int glfw_joystick_to_sf_joystick(int glfw_joystick);
	int glfw_gamepad_axis_to_sf_gamepad_axis(int glfw_axis);
}