#include "GameService.h"

#include "Engine/Engine.h"
#include "spdlog/spdlog.h"

GameService::GameService()
{
}

void GameService::init()
{
	Engine::get_engine()->get_event_system().subscribe_global<UpdateEvent>([this](UpdateEvent& e) { this->on_update(e); });
	Engine::get_engine()->get_event_system().subscribe_layer<KeyPressEvent>(Layer::Game, [this](KeyPressEvent& e) { this->on_key_press_event(e); });
	Engine::get_engine()->get_event_system().subscribe_global<MousePressEvent>([this](MousePressEvent& e) { this->on_mouse_press_event(e); });
}

void GameService::on_update(UpdateEvent& e)
{
	if (Engine::get_engine()->get_input_manager().get_key(SF_KEY_A))
	{
		double xpos = Engine::get_engine()->get_input_manager().get_cursorX();
		double ypos = Engine::get_engine()->get_input_manager().get_cursorY();
		spdlog::info("Mouse Position: X = {0}, Y = {1}", xpos, ypos);
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

