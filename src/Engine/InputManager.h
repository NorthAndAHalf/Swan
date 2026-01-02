#pragma once

#include "Window.h"

class InputManager
{
public:
	InputManager(Window* w);

	void set_window(Window* w);

	bool get_key(int keycode);
	bool get_mouse_button(int button);
	std::pair<double, double> get_cursor();
	double get_cursorX();
	double get_cursorY();

private:
	Window* mWindow;
};
