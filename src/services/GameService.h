#pragma once

#include "events/EventListener.h"

class GameService : public EventListener
{
public:
	GameService();

	void init();

	virtual void on_update() override;

	virtual void on_key_press_event(KeyPressEvent& e) override;
	virtual void on_key_release_event(KeyReleaseEvent& e) override;

	virtual void on_mouse_move_event(MouseMoveEvent& e) override;
	virtual void on_mouse_press_event(MousePressEvent& e) override;
	virtual void on_mouse_release_event(MouseReleaseEvent& e) override;
};
