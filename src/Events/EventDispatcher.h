#pragma once

#include "EventListener.h"
#include <vector>

class EventDispatcher
{

public:
	EventDispatcher();

	void subscribe(EventListener* l);
	void subscribe_overlay(EventListener* l);

	void dispatch_key_press(KeyPressEvent e);
	void dispatch_key_release(KeyReleaseEvent e);

	void dispatch_mouse_move(MouseMoveEvent e);
	void dispatch_mouse_press(MousePressEvent e);
	void dispatch_mouse_release(MouseReleaseEvent e);

	void dispatch_frame_start();
	void dispatch_frame_end();

	void dispatch_update();

private:
	std::vector<EventListener*> listeners;
	std::vector<EventListener*> overlay_listeners;
};
