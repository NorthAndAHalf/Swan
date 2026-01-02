#pragma once

#include "Events/Event.h"

class GameService
{
public:
	GameService();

	void init();

private:
	void on_update(UpdateEvent& e);
	void on_key_press_event(KeyPressEvent& e);
	void on_mouse_press_event(MousePressEvent& e);
};
