#pragma once

#include "Window.h"

class InputManager
{
public:
	InputManager(Window* w);
	void Init();

	void SetWindow(Window* w);

	bool GetKey(int keycode);
	bool GetMouseButton(int button);
	std::pair<double, double> GetMouse();
	double GetMouseX();
	double GetMouseY();

private:
	Window* mWindow;

	void OnImguiReleaseControl(ImGuiReleaseControlEvent& e);
	void OnImguiTakeControl(ImGuiTakeControlEvent& e);

	bool m_IsAcceptingInput = false;
};
