#pragma once

#include "Events/EventListener.h"
#include "imgui/imgui.h"

class ImGuiService : public EventListener
{
public:
	ImGuiService();

	void init();
	
	void shutdown();

	virtual void on_frame_start() override;
	virtual void on_frame_end() override;

	virtual void on_event(Event& e) override;
};
