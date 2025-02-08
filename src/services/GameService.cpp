#include "GameService.h"

#include "Engine/Engine.h"
#include "spdlog/spdlog.h"

GameService::GameService()
{
}

void GameService::init()
{
}

void GameService::update()
{
	if (Engine::get_engine()->get_input_manager()->get_key(65))
	{
		double xpos = Engine::get_engine()->get_input_manager()->get_cursorX();
		double ypos = Engine::get_engine()->get_input_manager()->get_cursorY();
		spdlog::info("Mouse Position: X = {0}, Y = {1}", xpos, ypos);
	}
}

void GameService::on_key_press_event(KeyPressEvent& e)
{
	if (e.keycode == 32)
	{
		spdlog::info("Jump");
	}
}

void GameService::on_key_release_event(KeyReleaseEvent& e)
{
}

void GameService::on_mouse_move_event(MouseMoveEvent& e)
{
}

void GameService::on_mouse_press_event(MousePressEvent& e)
{
}

void GameService::on_mouse_release_event(MouseReleaseEvent& e)
{
}
