#pragma once

#include "Event.h"
#include "spdlog/spdlog.h"

class EventListener
{
	friend class EventDispatcher;
private:
	virtual void on_event(Event& e) {}

	virtual void on_key_press_event(KeyPressEvent& e) {}
	virtual void on_key_release_event(KeyReleaseEvent& e) {}

	virtual void on_mouse_move_event(MouseMoveEvent& e) {}
	virtual void on_mouse_press_event(MousePressEvent& e) {}
	virtual void on_mouse_release_event(MouseReleaseEvent& e) {}

	virtual void on_frame_start() {}
	virtual void on_frame_end() {}

	virtual void on_update() {}
};
