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
	void on_frame_start(FrameStartEvent& e);
	void on_frame_end(FrameEndEvent& e);
	void update_key_modifiers(int mods);
    
    ImGuiIO* m_Io;
};

namespace ImGuiHelpers
{
	ImGuiKey sf_key_to_imgui_key(int key);
	ImGuiMouseButton sf_mouse_button_to_imgui_mouse_button(int button);
	ImGuiKey sf_gamepad_button_to_imgui_key(int button);
}