#include "EventDispatcher.h"

#include <algorithm>
#include "spdlog/spdlog.h"

EventDispatcher::EventDispatcher()
{
}

void EventDispatcher::subscribe(EventListener* l)
{
	if (std::find(listeners.begin(), listeners.end(), l) != listeners.end()) 
	{
		spdlog::warn("Duplicate event listener subcribed, rejecting subscription");
		return;
	}

	listeners.push_back(l);
}

// Could change this system to have a general event queue in the future
// Then have a dispatch function that calls the right callbacks according to each event type
// Not sure yet though

void EventDispatcher::dispatchKeyPress(KeyPressEvent e)
{
	for (EventListener* l : listeners)
	{
		l->onKeyPressEvent(e);
	}
}

void EventDispatcher::dispatchKeyRelease(KeyReleaseEvent e)
{
	for (EventListener* l : listeners)
	{
		l->onKeyReleaseEvent(e);
	}
}
