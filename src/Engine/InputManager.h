#pragma once

#include "Window.h"

class InputManager
{
public:
	InputManager(Window* w);
	void init();

	void set_window(Window* w);

	bool get_key(int keycode);
	bool get_mouse_button(int button);
	std::pair<double, double> get_mouse();
	double get_mouseX();
	double get_mouseY();

private:
	Window* mWindow;

	void on_imgui_release_control(ImGuiReleaseControlEvent& e);
	void on_imgui_take_control(ImGuiTakeControlEvent& e);

	bool m_IsAcceptingInput = false;
};
