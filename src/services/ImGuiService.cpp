#include "ImGuiService.h"

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

#include "Engine.h"

ImGuiService::ImGuiService()
{
}

// Still need to handle inputs to imgui, as at the moment they go to both imgui and game service
void ImGuiService::init()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

	Engine::get_engine()->get_primary_window()->init_imgui();
}

void ImGuiService::begin_frame()
{
	// (Your code process and dispatch Win32 messages)
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::ShowDemoWindow(); // Show demo window! :)3

	if (ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow))
	{
		Engine::get_engine()->block_inputs();
	}
}

void ImGuiService::end_frame()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	Engine::get_engine()->unblock_inputs();
}

void ImGuiService::shutdown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiService::on_event(Event& e)
{
	if (ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow))
	{
		e.handled = true;
	}
}
