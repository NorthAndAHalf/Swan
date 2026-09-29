#include "ImGuiService.h"

#include "glad/glad.h"

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_opengl3.h"

#include "spdlog/spdlog.h"
#include "../Engine/Engine.h"
#include "imgui_internal.h"

ImGuiService::ImGuiService()
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

    SW_EVENT_SUBSCRIBE(UpdateEvent, OnUpdate);
    SW_EVENT_SUBSCRIBE(WindowResizeEvent, OnWindowResize);
                  
    Engine::Input().RegisterDebugListener<KeyPressEvent>((InputListener*) this);
    Engine::Input().RegisterDebugListener<KeyReleaseEvent>((InputListener*) this);
    Engine::Input().RegisterDebugListener<CharEvent>((InputListener*) this);
    Engine::Input().RegisterDebugListener<MouseMoveEvent>((InputListener*) this);
    Engine::Input().RegisterDebugListener<MousePressEvent>((InputListener*) this);
    Engine::Input().RegisterDebugListener<MouseReleaseEvent>((InputListener*) this);
    Engine::Input().RegisterDebugListener<MouseWheelEvent>((InputListener*) this);
}

ImGuiService::~ImGuiService()
{
    Engine::Input().DeregisterDebugListener<KeyPressEvent>((InputListener*) this);
    Engine::Input().DeregisterDebugListener<KeyReleaseEvent>((InputListener*) this);
    Engine::Input().DeregisterDebugListener<CharEvent>((InputListener*) this);
    Engine::Input().DeregisterDebugListener<MouseMoveEvent>((InputListener*) this);
    Engine::Input().DeregisterDebugListener<MousePressEvent>((InputListener*) this);
    Engine::Input().DeregisterDebugListener<MouseReleaseEvent>((InputListener*) this);
    Engine::Input().DeregisterDebugListener<MouseWheelEvent>((InputListener*) this);
}

void ImGuiService::OnUpdate(const UpdateEvent& e)
{
    int frameBufferWidth;
    int frameBufferHeight;
    Engine::GetEngine().GetPrimaryWindow().GetFrameBufferSize(&frameBufferWidth, &frameBufferHeight);

    m_Io->DisplaySize = ImVec2((float) frameBufferWidth, (float) frameBufferHeight);

	float dt = Engine::Time().GetDeltaTime();

	m_Io->DeltaTime = (dt > 0.0f) ? dt : (1.0f / 60.0f); // Provide a default delta time (1 frame at 60fps) if dt is 0 (as it would be on first frame)
	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();
	ImGui::ShowDemoWindow();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        Engine::GetEngine().GetPrimaryWindow().SetOpenGLContext();
    }
}

void ImGuiService::OnWindowResize(const WindowResizeEvent& e)
{
    m_Io->DisplaySize = ImVec2((float)e.frameBufferWidth, (float)e.frameBufferHeight);
}

void ImGuiService::OnKeyPress(int key, int scancode, int mods) {
    UpdateKeyModifiers(mods);
    ImGuiKey imguiKey = ImGuiHelpers::SW_key_to_imgui_key(key);
    m_Io->AddKeyEvent(imguiKey, true);
}

void ImGuiService::OnKeyRepeat(int key, int scancode, int mods) {
    UpdateKeyModifiers(mods);
    ImGuiKey imguiKey = ImGuiHelpers::SW_key_to_imgui_key(key);
    m_Io->AddKeyEvent(imguiKey, true);
}

void ImGuiService::OnKeyRelease(int key, int scancode, int mods) {
    UpdateKeyModifiers(mods);
    ImGuiKey imguiKey = ImGuiHelpers::SW_key_to_imgui_key(key);
    m_Io->AddKeyEvent(imguiKey, false);
}

void ImGuiService::OnCharInput(unsigned int codepoint) {
    m_Io->AddInputCharacter(codepoint);
}

void ImGuiService::OnMouseMove(double xpos, double ypos) {
    float xScale, yScale;
    Engine::GetEngine().GetPrimaryWindow().GetContentScale(&xScale, &yScale);
    m_Io->AddMousePosEvent(xpos, ypos);
}

void ImGuiService::OnMousePress(int button, int mods) {
    UpdateKeyModifiers(mods);
    ImGuiMouseButton imguiButton = ImGuiHelpers::SW_mouse_button_to_imgui_mouse_button(button);
    m_Io->AddMouseButtonEvent(imguiButton, true);
}

void ImGuiService::OnMouseRelease(int button, int mods) {
    UpdateKeyModifiers(mods);
    ImGuiMouseButton imguiButton = ImGuiHelpers::SW_mouse_button_to_imgui_mouse_button(button);
    m_Io->AddMouseButtonEvent(imguiButton, false);
}

void ImGuiService::OnMouseWheel(double x_offset, double y_offset) {
    m_Io->AddMouseWheelEvent(x_offset, y_offset);
}

void ImGuiService::Shutdown()  
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiService::UpdateKeyModifiers(int mods)
{
    m_Io->AddKeyEvent(ImGuiMod_Shift, (mods & 0x0001) != 0); // GLFW_MOD_SHIFT
    m_Io->AddKeyEvent(ImGuiMod_Ctrl, (mods & 0x0002) != 0); // GLFW_MOD_CONTROL
    m_Io->AddKeyEvent(ImGuiMod_Alt, (mods & 0x0004) != 0); // GLFW_MOD_ALT
    m_Io->AddKeyEvent(ImGuiMod_Super, (mods & 0x0008) != 0); // GLFW_MOD_SUPER
}

ImGuiKey ImGuiHelpers::SW_key_to_imgui_key(int key)
{
    switch (key)
    {
    case SW_KEY_TAB: return ImGuiKey_Tab;
    case SW_KEY_LEFT: return ImGuiKey_LeftArrow;
    case SW_KEY_RIGHT: return ImGuiKey_RightArrow;
    case SW_KEY_UP: return ImGuiKey_UpArrow;
    case SW_KEY_DOWN: return ImGuiKey_DownArrow;
    case SW_KEY_PAGE_UP: return ImGuiKey_PageUp;
    case SW_KEY_PAGE_DOWN: return ImGuiKey_PageDown;
    case SW_KEY_HOME: return ImGuiKey_Home;
    case SW_KEY_END: return ImGuiKey_End;
    case SW_KEY_INSERT: return ImGuiKey_Insert;
    case SW_KEY_DELETE: return ImGuiKey_Delete;
    case SW_KEY_BACKSPACE: return ImGuiKey_Backspace;
    case SW_KEY_SPACE: return ImGuiKey_Space;
    case SW_KEY_ENTER: return ImGuiKey_Enter;
    case SW_KEY_ESCAPE: return ImGuiKey_Escape;
    case SW_KEY_APOSTROPHE: return ImGuiKey_Apostrophe;
    case SW_KEY_COMMA: return ImGuiKey_Comma;
    case SW_KEY_MINUS: return ImGuiKey_Minus;
    case SW_KEY_PERIOD: return ImGuiKey_Period;
    case SW_KEY_SLASH: return ImGuiKey_Slash;
    case SW_KEY_SEMICOLON: return ImGuiKey_Semicolon;
    case SW_KEY_EQUAL: return ImGuiKey_Equal;
    case SW_KEY_LEFT_BRACKET: return ImGuiKey_LeftBracket;
    case SW_KEY_BACKSLASH: return ImGuiKey_Backslash;
    case SW_KEY_RIGHT_BRACKET: return ImGuiKey_RightBracket;
    case SW_KEY_GRAVE_ACCENT: return ImGuiKey_GraveAccent;
    case SW_KEY_CAPS_LOCK: return ImGuiKey_CapsLock;
    case SW_KEY_SCROLL_LOCK: return ImGuiKey_ScrollLock;
    case SW_KEY_NUM_LOCK: return ImGuiKey_NumLock;
    case SW_KEY_PRINT_SCREEN: return ImGuiKey_PrintScreen;
    case SW_KEY_PAUSE: return ImGuiKey_Pause;
    case SW_KEY_KP_0: return ImGuiKey_Keypad0;
    case SW_KEY_KP_1: return ImGuiKey_Keypad1;
    case SW_KEY_KP_2: return ImGuiKey_Keypad2;
    case SW_KEY_KP_3: return ImGuiKey_Keypad3;
    case SW_KEY_KP_4: return ImGuiKey_Keypad4;
    case SW_KEY_KP_5: return ImGuiKey_Keypad5;
    case SW_KEY_KP_6: return ImGuiKey_Keypad6;
    case SW_KEY_KP_7: return ImGuiKey_Keypad7;
    case SW_KEY_KP_8: return ImGuiKey_Keypad8;
    case SW_KEY_KP_9: return ImGuiKey_Keypad9;
    case SW_KEY_KP_DECIMAL: return ImGuiKey_KeypadDecimal;
    case SW_KEY_KP_DIVIDE: return ImGuiKey_KeypadDivide;
    case SW_KEY_KP_MULTIPLY: return ImGuiKey_KeypadMultiply;
    case SW_KEY_KP_SUBTRACT: return ImGuiKey_KeypadSubtract;
    case SW_KEY_KP_ADD: return ImGuiKey_KeypadAdd;
    case SW_KEY_KP_ENTER: return ImGuiKey_KeypadEnter;
    case SW_KEY_KP_EQUAL: return ImGuiKey_KeypadEqual;
    case SW_KEY_LEFT_SHIFT: return ImGuiKey_LeftShift;
    case SW_KEY_LEFT_CONTROL: return ImGuiKey_LeftCtrl;
    case SW_KEY_LEFT_ALT: return ImGuiKey_LeftAlt;
    case SW_KEY_LEFT_SUPER: return ImGuiKey_LeftSuper;
    case SW_KEY_RIGHT_SHIFT: return ImGuiKey_RightShift;
    case SW_KEY_RIGHT_CONTROL: return ImGuiKey_RightCtrl;
    case SW_KEY_RIGHT_ALT: return ImGuiKey_RightAlt;
    case SW_KEY_RIGHT_SUPER: return ImGuiKey_RightSuper;
    case SW_KEY_MENU: return ImGuiKey_Menu;
    case SW_KEY_0: return ImGuiKey_0;
    case SW_KEY_1: return ImGuiKey_1;
    case SW_KEY_2: return ImGuiKey_2;
    case SW_KEY_3: return ImGuiKey_3;
    case SW_KEY_4: return ImGuiKey_4;
    case SW_KEY_5: return ImGuiKey_5;
    case SW_KEY_6: return ImGuiKey_6;
    case SW_KEY_7: return ImGuiKey_7;
    case SW_KEY_8: return ImGuiKey_8;
    case SW_KEY_9: return ImGuiKey_9;
    case SW_KEY_A: return ImGuiKey_A;
    case SW_KEY_B: return ImGuiKey_B;
    case SW_KEY_C: return ImGuiKey_C;
    case SW_KEY_D: return ImGuiKey_D;
    case SW_KEY_E: return ImGuiKey_E;
    case SW_KEY_F: return ImGuiKey_F;
    case SW_KEY_G: return ImGuiKey_G;
    case SW_KEY_H: return ImGuiKey_H;
    case SW_KEY_I: return ImGuiKey_I;
    case SW_KEY_J: return ImGuiKey_J;
    case SW_KEY_K: return ImGuiKey_K;
    case SW_KEY_L: return ImGuiKey_L;
    case SW_KEY_M: return ImGuiKey_M;
    case SW_KEY_N: return ImGuiKey_N;
    case SW_KEY_O: return ImGuiKey_O;
    case SW_KEY_P: return ImGuiKey_P;
    case SW_KEY_Q: return ImGuiKey_Q;
    case SW_KEY_R: return ImGuiKey_R;
    case SW_KEY_S: return ImGuiKey_S;
    case SW_KEY_T: return ImGuiKey_T;
    case SW_KEY_U: return ImGuiKey_U;
    case SW_KEY_V: return ImGuiKey_V;
    case SW_KEY_W: return ImGuiKey_W;
    case SW_KEY_X: return ImGuiKey_X;
    case SW_KEY_Y: return ImGuiKey_Y;
    case SW_KEY_Z: return ImGuiKey_Z;
    case SW_KEY_F1: return ImGuiKey_F1;
    case SW_KEY_F2: return ImGuiKey_F2;
    case SW_KEY_F3: return ImGuiKey_F3;
    case SW_KEY_F4: return ImGuiKey_F4;
    case SW_KEY_F5: return ImGuiKey_F5;
    case SW_KEY_F6: return ImGuiKey_F6;
    case SW_KEY_F7: return ImGuiKey_F7;
    case SW_KEY_F8: return ImGuiKey_F8;
    case SW_KEY_F9: return ImGuiKey_F9;
    case SW_KEY_F10: return ImGuiKey_F10;
    case SW_KEY_F11: return ImGuiKey_F11;
    case SW_KEY_F12: return ImGuiKey_F12;
    case SW_KEY_F13: return ImGuiKey_F13;
    case SW_KEY_F14: return ImGuiKey_F14;
    case SW_KEY_F15: return ImGuiKey_F15;
    case SW_KEY_F16: return ImGuiKey_F16;
    case SW_KEY_F17: return ImGuiKey_F17;
    case SW_KEY_F18: return ImGuiKey_F18;
    case SW_KEY_F19: return ImGuiKey_F19;
    case SW_KEY_F20: return ImGuiKey_F20;
    case SW_KEY_F21: return ImGuiKey_F21;
    case SW_KEY_F22: return ImGuiKey_F22;
    case SW_KEY_F23: return ImGuiKey_F23;
    case SW_KEY_F24: return ImGuiKey_F24;
    default: return ImGuiKey_None;
    }
}

// Mouse button mapping function
ImGuiMouseButton ImGuiHelpers::SW_mouse_button_to_imgui_mouse_button(int button)
{
    switch (button)
    {
    case SW_MOUSE_BUTTON_LEFT: return ImGuiMouseButton_Left;
    case SW_MOUSE_BUTTON_RIGHT: return ImGuiMouseButton_Right;
    case SW_MOUSE_BUTTON_MIDDLE: return ImGuiMouseButton_Middle;
    case SW_MOUSE_BUTTON_4: return 3; // ImGui supports up to 5 mouse buttons (0-4)
    case SW_MOUSE_BUTTON_5: return 4;
    default: return -1; // Invalid button
    }
}

// Gamepad button mapping function
ImGuiKey ImGuiHelpers::SW_gamepad_button_to_imgui_key(int button)
{
    switch (button)
    {
    case SW_GAMEPAD_BUTTON_A: return ImGuiKey_GamepadFaceDown;
    case SW_GAMEPAD_BUTTON_B: return ImGuiKey_GamepadFaceRight;
    case SW_GAMEPAD_BUTTON_X: return ImGuiKey_GamepadFaceLeft;
    case SW_GAMEPAD_BUTTON_Y: return ImGuiKey_GamepadFaceUp;
    case SW_GAMEPAD_BUTTON_LEFT_BUMPER: return ImGuiKey_GamepadL1;
    case SW_GAMEPAD_BUTTON_RIGHT_BUMPER: return ImGuiKey_GamepadR1;
    case SW_GAMEPAD_BUTTON_BACK: return ImGuiKey_GamepadBack;
    case SW_GAMEPAD_BUTTON_START: return ImGuiKey_GamepadStart;
    case SW_GAMEPAD_BUTTON_GUIDE: return ImGuiKey_None; // ImGui doesn't have a guide button
    case SW_GAMEPAD_BUTTON_LEFT_THUMB: return ImGuiKey_GamepadL3;
    case SW_GAMEPAD_BUTTON_RIGHT_THUMB: return ImGuiKey_GamepadR3;
    case SW_GAMEPAD_BUTTON_DPAD_UP: return ImGuiKey_GamepadDpadUp;
    case SW_GAMEPAD_BUTTON_DPAD_RIGHT: return ImGuiKey_GamepadDpadRight;
    case SW_GAMEPAD_BUTTON_DPAD_DOWN: return ImGuiKey_GamepadDpadDown;
    case SW_GAMEPAD_BUTTON_DPAD_LEFT: return ImGuiKey_GamepadDpadLeft;
    default: return ImGuiKey_None;
    }
}