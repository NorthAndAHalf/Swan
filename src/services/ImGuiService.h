#pragma once

#include "Events/Event.h"
#include "imgui/imgui.h"
#include "Core.h"

class ImGuiService
{
public:
	ImGuiService();

	void init();
	
	void shutdown();

private:
	void on_update(UpdateEvent& e);
	void on_window_resize(WindowResizeEvent& e);
	void update_key_modifiers(int mods);
	void on_key_press(KeyPressEvent& e);
	void on_key_release(KeyReleaseEvent& e);
	void on_char_input(CharEvent& e);
	void on_mouse_press(MousePressEvent& e);
	void on_mouse_release(MouseReleaseEvent& e);
	void on_mouse_wheel(MouseWheelEvent& e);
	void on_mouse_move(MouseMoveEvent& e);
    
	void release_user_control();
	void take_user_control();
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