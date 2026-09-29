#pragma once

#include "Events/Event.h"
#include "../Events/EventListener.h"
#include "../Engine/InputManager.h"
#include <deque>

struct Responder : public EventListener
{
	Responder();

	void OnKeyPress(const KeyPressEvent& e);
};

class GameService : public InputListener, public EventListener
{
public:
	GameService();
	~GameService();

private:
	void OnUpdate(const UpdateEvent& e);
	void OnKeyPress(int key, int scancode, int mods) override;
	void OnMousePress(int button, int mods) override;

	std::deque<Responder> responders;
};
