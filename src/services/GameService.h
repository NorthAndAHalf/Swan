#pragma once

#include "Events/Event.h"
#include "../Engine/Engine.h"

class GameService : public InputListener
{
public:
	GameService();
	~GameService();

private:
	void OnUpdate(const UpdateEvent& e);
	void OnKeyPress(int key, int scancode, int mods) override;
	void OnMousePress(int button, int mods) override;
};
