#include "GameService.h"

#include "Engine/Engine.h"
#include "spdlog/spdlog.h"

GameService::GameService()
{
}

void GameService::init()
{
	Engine::events().subscribe_global<UpdateEvent, GameService, &GameService::on_update>(this);
	Engine::events().subscribe<KeyPressEvent, GameService, &GameService::on_key_press_event>(Layer::GAME, this);
	Engine::events().subscribe_global<MousePressEvent, GameService, &GameService::on_mouse_press_event>(this);
}

void GameService::on_update(UpdateEvent& e)
{
	if (Engine::input().get_key(SF_KEY_A))
	{
		double xpos = Engine::input().get_mouseX();
		double ypos = Engine::input().get_mouseY();
		spdlog::trace("Mouse Position: X = {0}, Y = {1}", xpos, ypos);
	}
}

void GameService::on_key_press_event(KeyPressEvent& e)
{
	if (e.keycode == SF_KEY_SPACE)
	{
		spdlog::info("Jump");
	}
}

void GameService::on_mouse_press_event(MousePressEvent& e)
{
	spdlog::info(e.button);
}

