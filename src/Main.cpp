#include "spdlog/spdlog.h"
#include "Engine/Engine.h"
#include "services/GameService.h"
#include "services/ImGuiService.h"

#ifdef SF_DEBUG
#define SPD_DEBUG_LEVEL(x) x;
#else
#define SPD_DEBUG_LEVEL(x);
#endif

int main()
{
	// Setting up spdlog level, spdlog::trace() will only print when in debug configuration, other log functions will work in all configurations
	SPD_DEBUG_LEVEL(spdlog::set_level(spdlog::level::trace));
	
	try
	{
		Engine::get_engine()->init();
	}
	catch (const std::exception& e)
	{
		spdlog::critical(e.what());
		return -1;
	}

	ImGuiService* imguiService = new ImGuiService();
	imguiService->init();
	GameService* gameService = new GameService();
	gameService->init();

	try
	{
		Engine::get_engine()->start_main_loop();
	}
	catch (const std::exception& e)
	{
		spdlog::critical(e.what());
		return -1;
	}

	Engine::get_engine()->shutdown();
}
