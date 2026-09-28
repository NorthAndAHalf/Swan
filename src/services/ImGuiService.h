#pragma once

#include "Events/Event.h"
#include "imgui/imgui.h"
#include "Core.h"
#include "../Engine/Engine.h"

class ImGuiService : public InputListener
{
public:
	ImGuiService();
	~ImGuiService();
	
	void Shutdown();

	void OnUpdate(const UpdateEvent& e);
	void OnWindowResize(const WindowResizeEvent& e);
	void UpdateKeyModifiers(int mods);
	void OnKeyPress(int key, int scancode, int mods) override;
	void OnKeyRepeat(int key, int scancode, int mods) override;
	void OnKeyRelease(int key, int scancode, int mods) override;
	void OnCharInput(unsigned int codepoint) override;
	void OnMouseMove(double xpos, double ypos) override;
	void OnMousePress(int button, int mods) override;
	void OnMouseRelease(int button, int mods) override;
	void OnMouseWheel(double x_offset, double y_offset) override;

    ImGuiIO* m_Io;
};

namespace ImGuiHelpers
{
	ImGuiKey SW_key_to_imgui_key(int key);
	ImGuiMouseButton SW_mouse_button_to_imgui_mouse_button(int button);
	ImGuiKey SW_gamepad_button_to_imgui_key(int button);
}