#pragma once

#include "Events/Event.h"

class GameService
{
public:
	GameService();

	void Init();

private:
	void OnUpdate(UpdateEvent& e);
	void OnKeyPress(KeyPressEvent& e);
	void OnMousePress(MousePressEvent& e);
};
