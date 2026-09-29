#include "GameService.h"
#include "../Engine/Engine.h"
#include "spdlog/spdlog.h"
Responder::Responder()
{
	SW_EVENT_SUBSCRIBE(KeyPressEvent, OnKeyPress);
}

void Responder::OnKeyPress(const KeyPressEvent& e)
{
	spdlog::info("Response: {0}", e.keycode);
}

GameService::GameService()
{
	SW_EVENT_SUBSCRIBE(UpdateEvent, OnUpdate);

	Engine::Input().RegisterListener<KeyPressEvent>((InputListener*) this);
	Engine::Input().RegisterListener<MousePressEvent>((InputListener*) this);
}

GameService::~GameService()
{
	Engine::Input().DeregisterListener<KeyPressEvent>((InputListener*) this);
	Engine::Input().DeregisterListener<MousePressEvent>((InputListener*) this);
}

void GameService::OnUpdate(const UpdateEvent& e)
{
	if (Engine::Input().GetKey(SW_KEY_A))
	{
		double xpos = Engine::Input().GetMouseX();
		double ypos = Engine::Input().GetMouseY();
		spdlog::trace("Mouse Position: X = {0}, Y = {1}", xpos, ypos);
	}
}

void GameService::OnKeyPress(int key, int scancode, int mods)
{
	if (key == SW_KEY_SPACE)
	{
		spdlog::info("Jump");
	}
	if (key == SW_KEY_R)
	{
		responders.emplace_back();
	}
	if (key == SW_KEY_T)
	{
		if (responders.empty())
		{
			spdlog::warn("No responders here");
		}
		else
		{
			responders.pop_back();
		}
	}
}

void GameService::OnMousePress(int button, int mods)
{
}

