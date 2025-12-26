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
	ImGuiIO& io = ImGui::GetIO();

	io.DisplaySize = ImVec2(
		(float) Engine::get_engine()->get_primary_window().get_width(),
		(float) Engine::get_engine()->get_primary_window().get_height());

	float dt = Engine::get_engine()->get_time_manager().get_delta_time();

	io.DeltaTime = (dt > 0.0f) ? dt : (1.0f / 60.0f); // Provide a default delta time (1 frame at 60fps) if dt is 0 (as it would be on first frame)

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	ImGui_ImplOpenGL3_Init();
}

void ImGuiService::on_frame_start()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();
	ImGui::ShowDemoWindow();

	if (ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow))
	{
		Engine::get_engine()->get_input_manager().block_inputs();
	}
}

void ImGuiService::on_frame_end()
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

	Engine::get_engine()->get_input_manager().unblock_inputs();
}

void ImGuiService::shutdown()  
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiService::on_event(Event& e)
{
	if (ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow))
	{
		e.handled = true;
	}
}
