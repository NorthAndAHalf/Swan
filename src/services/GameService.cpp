#include "GameService.h"

#include "Engine/Engine.h"
#include "spdlog/spdlog.h"

GameService::GameService()
{
}

void GameService::init()
{
	Engine::get_engine()->get_event_system().subscribe_global<UpdateEvent, GameService, &GameService::on_update>(this);
	Engine::get_engine()->get_event_system().subscribe<KeyPressEvent, GameService, &GameService::on_key_press_event>(Layer::GAME, this);
	Engine::get_engine()->get_event_system().subscribe_global<MousePressEvent, GameService, &GameService::on_mouse_press_event>(this);
}

void GameService::on_update(UpdateEvent& e)
{
	if (Engine::get_engine()->get_input_manager().get_key(SF_KEY_A))
	{
		double xpos = Engine::get_engine()->get_input_manager().get_cursorX();
		double ypos = Engine::get_engine()->get_input_manager().get_cursorY();
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

