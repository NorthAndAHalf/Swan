#pragma once

#include "Events/EventListener.h"

class ImGuiService : public EventListener
{
public:
	ImGuiService();

	void init();
	void begin_frame();
	void end_frame();
	void shutdown();

	virtual void on_event(Event& e);
};
