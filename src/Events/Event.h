#pragma once

class Event
{
public:
	Event() {}
};

class KeyPressEvent : public Event
{
public:
	KeyPressEvent(int _keycode, int _scancode, int _mods);

	const int keycode;
	const int scancode;
	const int mods;
};

class KeyReleaseEvent : public Event
{
public:
	KeyReleaseEvent(int _keycode, int _scancode, int _mods);

	const int keycode;
	const int scancode;
	const int mods;
};
