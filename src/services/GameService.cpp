#include "GameService.h"

#include "Engine/Engine.h"
#include "spdlog/spdlog.h"

GameService::GameService()
{
}

void GameService::Init()
{
	Engine::Events().SubscribeGlobal<UpdateEvent, GameService, &GameService::OnUpdate>(this);
	Engine::Events().Subscribe<KeyPressEvent, GameService, &GameService::OnKeyPress>(Layer::GAME, this);
	Engine::Events().SubscribeGlobal<MousePressEvent, GameService, &GameService::OnMousePress>(this);
}

void GameService::OnUpdate(UpdateEvent& e)
{
	if (Engine::Input().GetKey(SF_KEY_A))
	{
		double xpos = Engine::Input().GetMouseX();
		double ypos = Engine::Input().GetMouseY();
		spdlog::trace("Mouse Position: X = {0}, Y = {1}", xpos, ypos);
	}
}

void GameService::OnKeyPress(KeyPressEvent& e)
{
	if (e.keycode == SF_KEY_SPACE)
	{
		spdlog::info("Jump");
	}
}

void GameService::OnMousePress(MousePressEvent& e)
{
	spdlog::info(e.button);
}

