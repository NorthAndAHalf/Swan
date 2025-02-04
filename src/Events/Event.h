#pragma once

class Event
{
public:
	Event() {}
	bool handled = false;
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

class MouseMoveEvent : public Event
{
public:
	MouseMoveEvent(double _xpos, double _ypos);

	const double xpos;
	const double ypos;
};

class MousePressEvent : public Event
{
public:
	MousePressEvent(int _button, int _mods);

	const int button;
	const int mods;
};

class MouseReleaseEvent : public Event
{
public:
	MouseReleaseEvent(int _button, int _mods);

	const int button;
	const int mods;
};
