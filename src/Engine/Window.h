#pragma once
#include <stdint.h>
#include "GLFW/glfw3.h"
#include "Events/eventsystem.h"

class Window
{
public:
	Window(const char* startTitle, uint32_t startWidth, uint32_t startHeight, bool startFullscreen);

	void Init();
	void Destroy();
	void Update();

	void SetOpenGLContext();

	void SetTitle(const char* newTitle) { title = newTitle; }
	const char* GetTitle() { return title; }

	void SetHeight(uint32_t newHeight) { 
		height = newHeight; 
	}

	uint32_t GetHeight() { 
		return (isFullscreen) ? fullscreenHeight : height;
	}

	void SetWidth(uint32_t newWidth) { 
		width = newWidth; 
	}

	uint32_t GetWidth() { 
		return (isFullscreen) ? fullscreenWidth : width; 
	}

	void EnableCursor();
	void DisableCursor();

	void GetFrameBufferSize(int* width, int* height);
	void GetContentScale(float* x, float* y);

	void SetFullscreenHeight(uint32_t height) { fullscreenHeight = height; }
	void SetFullscreenWidth(uint32_t width) { fullscreenWidth = width;  }

	bool IsFullscreen() { return isFullscreen; }
	void MakeFullscreen();
	void MakeWindowed();
	void ToggleFullscreen();

	bool WindowShouldClose() { return glfwWindowShouldClose(glfwWindow); }

	// Input Polling
	bool GetKey(int keycode);
	bool GetMouseButton(int button);
	void GetMouseDelta(double* xpos, double* ypos);

private:
	const char* title;
	uint32_t height;
	uint32_t width;

	uint32_t fullscreenHeight;
	uint32_t fullscreenWidth;

	bool isFullscreen;

	GLFWwindow* glfwWindow;

	double m_LastX;
	double m_LastY;

	void SetEventCallbacks();
};

namespace GLFWHelpers {
	// Key and Button Mappings
	int glfw_key_to_SW_key(int glfw_key);
	int glfw_mouse_button_to_SW_mouse_button(int glfw_button);
	int glfw_mods_to_SW_mods(int glfw_mods);
	int glfw_gamepad_button_to_SW_gamepad_button(int glfw_button);

	// Joystick and Axis Mappings
	int glfw_joystick_to_SW_joystick(int glfw_joystick);
	int glfw_gamepad_axis_to_SW_gamepad_axis(int glfw_axis);
}