#include "GameService.h"

#include "spdlog/spdlog.h"

GameService::GameService()
{
	Engine::Events().Subscribe<UpdateEvent>(SW_BIND_CALLBACK(GameService, OnUpdate));

	Engine::Input().RegisterListener<KeyPressEvent>((InputListener*) this);
	Engine::Input().RegisterListener<MousePressEvent>((InputListener*) this);
}

GameService::~GameService()
{
	Engine::Events().Unsubscribe<UpdateEvent>(SW_BIND_CALLBACK(GameService, OnUpdate));

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
}

void GameService::OnMousePress(int button, int mods)
{
}

