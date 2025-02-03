#pragma once

#include "Event.h"
#include "spdlog/spdlog.h"

class EventListener
{
	friend class EventDispatcher;
private:
	virtual void onKeyPressEvent(KeyPressEvent& e) {}
	virtual void onKeyReleaseEvent(KeyReleaseEvent& e) {}
};
