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

void EventDispatcher::subscribe_overlay(EventListener* l)
{
	if (std::find(overlay_listeners.begin(), overlay_listeners.end(), l) != overlay_listeners.end())
	{
		spdlog::warn("Duplicate overlay event listener subcribed, rejecting subscription");
		return;
	}

	overlay_listeners.push_back(l);
}

// Could change this system to have a general event queue in the future
// Then have a dispatch function that calls the right callbacks according to each event type
// Not sure yet though

void EventDispatcher::dispatch_key_press(KeyPressEvent e)
{
	for (EventListener* l : overlay_listeners)
	{
		l->on_event(e);
		l->on_key_press_event(e);
	}

	if (e.handled) return;

	for (EventListener* l : listeners)
	{
		l->on_event(e);
		l->on_key_press_event(e);
	}
}

void EventDispatcher::dispatch_key_release(KeyReleaseEvent e)
{
	for (EventListener* l : overlay_listeners)
	{
		l->on_event(e);
		l->on_key_release_event(e);
	}

	if (e.handled) return;

	for (EventListener* l : listeners)
	{
		l->on_event(e);
		l->on_key_release_event(e);
	}
}

void EventDispatcher::dispatch_mouse_move(MouseMoveEvent e)
{
	for (EventListener* l : overlay_listeners)
	{
		l->on_event(e);
		l->on_mouse_move_event(e);
	}

	if (e.handled) return;

	for (EventListener* l : listeners)
	{
		l->on_event(e);
		l->on_mouse_move_event(e);
	}
}

void EventDispatcher::dispatch_mouse_press(MousePressEvent e)
{
	for (EventListener* l : overlay_listeners)
	{
		l->on_event(e);
		l->on_mouse_press_event(e);
	}

	if (e.handled) return;

	for (EventListener* l : listeners)
	{
		l->on_event(e);
		l->on_mouse_press_event(e);
	}
}

void EventDispatcher::dispatch_mouse_release(MouseReleaseEvent e)
{
	for (EventListener* l : overlay_listeners)
	{
		l->on_event(e);
		l->on_mouse_release_event(e);
	}

	if (e.handled) return;

	for (EventListener* l : listeners)
	{
		l->on_event(e);
		l->on_mouse_release_event(e);
	}
}

void EventDispatcher::dispatch_frame_start()
{
	for (EventListener* l : overlay_listeners)
	{
		l->on_frame_start();
	}

	for (EventListener* l : listeners)
	{
		l->on_frame_start();
	}
}

void EventDispatcher::dispatch_frame_end()
{
	for (EventListener* l : overlay_listeners)
	{
		l->on_frame_end();
	}

	for (EventListener* l : listeners)
	{
		l->on_frame_end();
	}
}

void EventDispatcher::dispatch_update()
{
	for (EventListener* l : overlay_listeners)
	{
		l->on_update();
	}

	for (EventListener* l : listeners)
	{
		l->on_update();
	}
}
