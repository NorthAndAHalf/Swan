#pragma once

#include "EventListener.h"
#include <vector>

class EventDispatcher
{

public:
	EventDispatcher();

	void subscribe(EventListener* l);
	void dispatchKeyPress(KeyPressEvent e);
	void dispatchKeyRelease(KeyReleaseEvent e);

private:
	std::vector<EventListener*> listeners;
};