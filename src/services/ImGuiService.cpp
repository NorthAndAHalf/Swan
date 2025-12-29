#include "ImGuiService.h"

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_opengl3.h"

#include "Engine/Engine.h"

ImGuiService::ImGuiService()
{
	
}

// Still need to handle inputs to imgui, as at the moment they go to both imgui and game service
void ImGuiService::init()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	m_Io = &ImGui::GetIO();

	m_Io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	m_Io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	m_Io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

	// --- NOTE ---
	// Viewports currently will not work, because ImGui is decoupled from GLFW, and viewports requires new Windows to be created
	// I think the best way to address this in the future would be to create a Window manager that ImGui can interface with
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	ImGui_ImplOpenGL3_Init();

	// Subscribe to events
	
	// Events handled in this file
	Engine::get_engine()->get_event_system().subscribe_global<FrameStartEvent>([this](FrameStartEvent& e) 
		{ this->on_frame_start(e); });

	Engine::get_engine()->get_event_system().subscribe_global<FrameEndEvent>([this](FrameEndEvent& e) 
		{ this->on_frame_end(e); });

	// Events handled by ImGui
	Engine::get_engine()->get_event_system().subscribe_global<KeyPressEvent>([this](KeyPressEvent& e)
		{
			if (!m_Io->WantCaptureKeyboard) return;

			ImGuiKey key = ImGuiHelpers::sf_key_to_imgui_key(e.keycode);
			this->m_Io->AddKeyEvent(key, true);
			e.handled = true;
		});

	Engine::get_engine()->get_event_system().subscribe_global<KeyReleaseEvent>([this](KeyReleaseEvent& e)
		{
			if (!m_Io->WantCaptureKeyboard) return;

			ImGuiKey key = ImGuiHelpers::sf_key_to_imgui_key(e.keycode);
			this->m_Io->AddKeyEvent(key, false);
			e.handled = true;
		});

	Engine::get_engine()->get_event_system().subscribe_global<CharEvent>([this](CharEvent& e)
		{
			if (!m_Io->WantCaptureKeyboard) return;

			this->m_Io->AddInputCharacter(e.codePoint);
			e.handled = true;
		});

	Engine::get_engine()->get_event_system().subscribe_global<MousePressEvent>([this](MousePressEvent& e)
		{
			if (!m_Io->WantCaptureMouse) return;

			ImGuiMouseButton button = ImGuiHelpers::sf_mouse_button_to_imgui_mouse_button(e.button);
			this->m_Io->AddMouseButtonEvent(button, true);
			e.handled = true;
		});

	Engine::get_engine()->get_event_system().subscribe_global<MouseReleaseEvent>([this](MouseReleaseEvent& e)
		{
			if (!m_Io->WantCaptureMouse) return;

			ImGuiMouseButton button = ImGuiHelpers::sf_mouse_button_to_imgui_mouse_button(e.button);
			this->m_Io->AddMouseButtonEvent(button, false);
			e.handled = true;
		});

	Engine::get_engine()->get_event_system().subscribe_global<MouseWheelEvent>([this](MouseWheelEvent& e)
		{
			if (!m_Io->WantCaptureMouse) return;

			this->m_Io->AddMouseWheelEvent(e.x_offset, e.y_offset);
			e.handled = true;
		});

	Engine::get_engine()->get_event_system().subscribe_global<MouseMoveEvent>([this](MouseMoveEvent& e)
		{
			if (!m_Io->WantCaptureMouse) return;

			this->m_Io->AddMousePosEvent(e.xpos, e.ypos);
			e.handled = true;
		});
}

void ImGuiService::on_frame_start(FrameStartEvent& e)
{
	m_Io->DisplaySize = ImVec2(
		(float)Engine::get_engine()->get_primary_window().get_width(),
		(float)Engine::get_engine()->get_primary_window().get_height());

	float dt = Engine::get_engine()->get_time_manager().get_delta_time();

	m_Io->DeltaTime = (dt > 0.0f) ? dt : (1.0f / 60.0f); // Provide a default delta time (1 frame at 60fps) if dt is 0 (as it would be on first frame)
	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();
	ImGui::ShowDemoWindow();
}

void ImGuiService::on_frame_end(FrameEndEvent& e)
{
	
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// Update and Render additional Platform Windows
	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		Engine::get_engine()->get_primary_window().set_opengl_context();
	}
}

void ImGuiService::shutdown()  
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiService::on_event_common(Event& e)
{
	if (ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow))
	{
		e.handled = true;
	}
}

ImGuiKey ImGuiHelpers::sf_key_to_imgui_key(int key)
{
    switch (key)
    {
    case SF_KEY_TAB: return ImGuiKey_Tab;
    case SF_KEY_LEFT: return ImGuiKey_LeftArrow;
    case SF_KEY_RIGHT: return ImGuiKey_RightArrow;
    case SF_KEY_UP: return ImGuiKey_UpArrow;
    case SF_KEY_DOWN: return ImGuiKey_DownArrow;
    case SF_KEY_PAGE_UP: return ImGuiKey_PageUp;
    case SF_KEY_PAGE_DOWN: return ImGuiKey_PageDown;
    case SF_KEY_HOME: return ImGuiKey_Home;
    case SF_KEY_END: return ImGuiKey_End;
    case SF_KEY_INSERT: return ImGuiKey_Insert;
    case SF_KEY_DELETE: return ImGuiKey_Delete;
    case SF_KEY_BACKSPACE: return ImGuiKey_Backspace;
    case SF_KEY_SPACE: return ImGuiKey_Space;
    case SF_KEY_ENTER: return ImGuiKey_Enter;
    case SF_KEY_ESCAPE: return ImGuiKey_Escape;
    case SF_KEY_APOSTROPHE: return ImGuiKey_Apostrophe;
    case SF_KEY_COMMA: return ImGuiKey_Comma;
    case SF_KEY_MINUS: return ImGuiKey_Minus;
    case SF_KEY_PERIOD: return ImGuiKey_Period;
    case SF_KEY_SLASH: return ImGuiKey_Slash;
    case SF_KEY_SEMICOLON: return ImGuiKey_Semicolon;
    case SF_KEY_EQUAL: return ImGuiKey_Equal;
    case SF_KEY_LEFT_BRACKET: return ImGuiKey_LeftBracket;
    case SF_KEY_BACKSLASH: return ImGuiKey_Backslash;
    case SF_KEY_RIGHT_BRACKET: return ImGuiKey_RightBracket;
    case SF_KEY_GRAVE_ACCENT: return ImGuiKey_GraveAccent;
    case SF_KEY_CAPS_LOCK: return ImGuiKey_CapsLock;
    case SF_KEY_SCROLL_LOCK: return ImGuiKey_ScrollLock;
    case SF_KEY_NUM_LOCK: return ImGuiKey_NumLock;
    case SF_KEY_PRINT_SCREEN: return ImGuiKey_PrintScreen;
    case SF_KEY_PAUSE: return ImGuiKey_Pause;
    case SF_KEY_KP_0: return ImGuiKey_Keypad0;
    case SF_KEY_KP_1: return ImGuiKey_Keypad1;
    case SF_KEY_KP_2: return ImGuiKey_Keypad2;
    case SF_KEY_KP_3: return ImGuiKey_Keypad3;
    case SF_KEY_KP_4: return ImGuiKey_Keypad4;
    case SF_KEY_KP_5: return ImGuiKey_Keypad5;
    case SF_KEY_KP_6: return ImGuiKey_Keypad6;
    case SF_KEY_KP_7: return ImGuiKey_Keypad7;
    case SF_KEY_KP_8: return ImGuiKey_Keypad8;
    case SF_KEY_KP_9: return ImGuiKey_Keypad9;
    case SF_KEY_KP_DECIMAL: return ImGuiKey_KeypadDecimal;
    case SF_KEY_KP_DIVIDE: return ImGuiKey_KeypadDivide;
    case SF_KEY_KP_MULTIPLY: return ImGuiKey_KeypadMultiply;
    case SF_KEY_KP_SUBTRACT: return ImGuiKey_KeypadSubtract;
    case SF_KEY_KP_ADD: return ImGuiKey_KeypadAdd;
    case SF_KEY_KP_ENTER: return ImGuiKey_KeypadEnter;
    case SF_KEY_KP_EQUAL: return ImGuiKey_KeypadEqual;
    case SF_KEY_LEFT_SHIFT: return ImGuiKey_LeftShift;
    case SF_KEY_LEFT_CONTROL: return ImGuiKey_LeftCtrl;
    case SF_KEY_LEFT_ALT: return ImGuiKey_LeftAlt;
    case SF_KEY_LEFT_SUPER: return ImGuiKey_LeftSuper;
    case SF_KEY_RIGHT_SHIFT: return ImGuiKey_RightShift;
    case SF_KEY_RIGHT_CONTROL: return ImGuiKey_RightCtrl;
    case SF_KEY_RIGHT_ALT: return ImGuiKey_RightAlt;
    case SF_KEY_RIGHT_SUPER: return ImGuiKey_RightSuper;
    case SF_KEY_MENU: return ImGuiKey_Menu;
    case SF_KEY_0: return ImGuiKey_0;
    case SF_KEY_1: return ImGuiKey_1;
    case SF_KEY_2: return ImGuiKey_2;
    case SF_KEY_3: return ImGuiKey_3;
    case SF_KEY_4: return ImGuiKey_4;
    case SF_KEY_5: return ImGuiKey_5;
    case SF_KEY_6: return ImGuiKey_6;
    case SF_KEY_7: return ImGuiKey_7;
    case SF_KEY_8: return ImGuiKey_8;
    case SF_KEY_9: return ImGuiKey_9;
    case SF_KEY_A: return ImGuiKey_A;
    case SF_KEY_B: return ImGuiKey_B;
    case SF_KEY_C: return ImGuiKey_C;
    case SF_KEY_D: return ImGuiKey_D;
    case SF_KEY_E: return ImGuiKey_E;
    case SF_KEY_F: return ImGuiKey_F;
    case SF_KEY_G: return ImGuiKey_G;
    case SF_KEY_H: return ImGuiKey_H;
    case SF_KEY_I: return ImGuiKey_I;
    case SF_KEY_J: return ImGuiKey_J;
    case SF_KEY_K: return ImGuiKey_K;
    case SF_KEY_L: return ImGuiKey_L;
    case SF_KEY_M: return ImGuiKey_M;
    case SF_KEY_N: return ImGuiKey_N;
    case SF_KEY_O: return ImGuiKey_O;
    case SF_KEY_P: return ImGuiKey_P;
    case SF_KEY_Q: return ImGuiKey_Q;
    case SF_KEY_R: return ImGuiKey_R;
    case SF_KEY_S: return ImGuiKey_S;
    case SF_KEY_T: return ImGuiKey_T;
    case SF_KEY_U: return ImGuiKey_U;
    case SF_KEY_V: return ImGuiKey_V;
    case SF_KEY_W: return ImGuiKey_W;
    case SF_KEY_X: return ImGuiKey_X;
    case SF_KEY_Y: return ImGuiKey_Y;
    case SF_KEY_Z: return ImGuiKey_Z;
    case SF_KEY_F1: return ImGuiKey_F1;
    case SF_KEY_F2: return ImGuiKey_F2;
    case SF_KEY_F3: return ImGuiKey_F3;
    case SF_KEY_F4: return ImGuiKey_F4;
    case SF_KEY_F5: return ImGuiKey_F5;
    case SF_KEY_F6: return ImGuiKey_F6;
    case SF_KEY_F7: return ImGuiKey_F7;
    case SF_KEY_F8: return ImGuiKey_F8;
    case SF_KEY_F9: return ImGuiKey_F9;
    case SF_KEY_F10: return ImGuiKey_F10;
    case SF_KEY_F11: return ImGuiKey_F11;
    case SF_KEY_F12: return ImGuiKey_F12;
    case SF_KEY_F13: return ImGuiKey_F13;
    case SF_KEY_F14: return ImGuiKey_F14;
    case SF_KEY_F15: return ImGuiKey_F15;
    case SF_KEY_F16: return ImGuiKey_F16;
    case SF_KEY_F17: return ImGuiKey_F17;
    case SF_KEY_F18: return ImGuiKey_F18;
    case SF_KEY_F19: return ImGuiKey_F19;
    case SF_KEY_F20: return ImGuiKey_F20;
    case SF_KEY_F21: return ImGuiKey_F21;
    case SF_KEY_F22: return ImGuiKey_F22;
    case SF_KEY_F23: return ImGuiKey_F23;
    case SF_KEY_F24: return ImGuiKey_F24;
    default: return ImGuiKey_None;
    }
}

// Mouse button mapping function
ImGuiMouseButton ImGuiHelpers::sf_mouse_button_to_imgui_mouse_button(int button)
{
    switch (button)
    {
    case SF_MOUSE_BUTTON_LEFT: return ImGuiMouseButton_Left;
    case SF_MOUSE_BUTTON_RIGHT: return ImGuiMouseButton_Right;
    case SF_MOUSE_BUTTON_MIDDLE: return ImGuiMouseButton_Middle;
    case SF_MOUSE_BUTTON_4: return 3; // ImGui supports up to 5 mouse buttons (0-4)
    case SF_MOUSE_BUTTON_5: return 4;
    default: return -1; // Invalid button
    }
}

// Gamepad button mapping function
ImGuiKey ImGuiHelpers::sf_gamepad_button_to_imgui_key(int button)
{
    switch (button)
    {
    case SF_GAMEPAD_BUTTON_A: return ImGuiKey_GamepadFaceDown;
    case SF_GAMEPAD_BUTTON_B: return ImGuiKey_GamepadFaceRight;
    case SF_GAMEPAD_BUTTON_X: return ImGuiKey_GamepadFaceLeft;
    case SF_GAMEPAD_BUTTON_Y: return ImGuiKey_GamepadFaceUp;
    case SF_GAMEPAD_BUTTON_LEFT_BUMPER: return ImGuiKey_GamepadL1;
    case SF_GAMEPAD_BUTTON_RIGHT_BUMPER: return ImGuiKey_GamepadR1;
    case SF_GAMEPAD_BUTTON_BACK: return ImGuiKey_GamepadBack;
    case SF_GAMEPAD_BUTTON_START: return ImGuiKey_GamepadStart;
    case SF_GAMEPAD_BUTTON_GUIDE: return ImGuiKey_None; // ImGui doesn't have a guide button
    case SF_GAMEPAD_BUTTON_LEFT_THUMB: return ImGuiKey_GamepadL3;
    case SF_GAMEPAD_BUTTON_RIGHT_THUMB: return ImGuiKey_GamepadR3;
    case SF_GAMEPAD_BUTTON_DPAD_UP: return ImGuiKey_GamepadDpadUp;
    case SF_GAMEPAD_BUTTON_DPAD_RIGHT: return ImGuiKey_GamepadDpadRight;
    case SF_GAMEPAD_BUTTON_DPAD_DOWN: return ImGuiKey_GamepadDpadDown;
    case SF_GAMEPAD_BUTTON_DPAD_LEFT: return ImGuiKey_GamepadDpadLeft;
    default: return ImGuiKey_None;
    }
}