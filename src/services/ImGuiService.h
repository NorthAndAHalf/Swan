#pragma once

#include "Events/Event.h"
#include "imgui/imgui.h"
#include "Core.h"

class ImGuiService
{
public:
	ImGuiService();

	void Init();
	
	void Shutdown();

private:
	void OnUpdate(UpdateEvent& e);
	void OnWindowResize(WindowResizeEvent& e);
	void UpdateKeyModifiers(int mods);
	void OnKeyPress(KeyPressEvent& e);
	void OnKeyRelease(KeyReleaseEvent& e);
	void OnCharInput(CharEvent& e);
	void OnMousePress(MousePressEvent& e);
	void OnMouseRelease(MouseReleaseEvent& e);
	void OnMouseWheel(MouseWheelEvent& e);
	void OnMouseMove(MouseMoveEvent& e);
    
	void ReleaseUserControl();
	void TakeUserControl();
	bool m_HasUserControl;

    ImGuiIO* m_Io;
	bool m_ImGuiUsedEscape = false;
};

namespace ImGuiHelpers
{
	ImGuiKey sf_key_to_imgui_key(int key);
	ImGuiMouseButton sf_mouse_button_to_imgui_mouse_button(int button);
	ImGuiKey sf_gamepad_button_to_imgui_key(int button);
}