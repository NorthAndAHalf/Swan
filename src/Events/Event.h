#pragma once

#include <cstdint>

// Base Event class and common derived classes are defined here, but custom events derived from the Base can be created in other files

class Event
{
	friend class EventSystem;
public:
	virtual ~Event() = default;
	bool handled = false;
protected:
	uint32_t typeId = 0; // Used by the event dispatcher to map event types to buckets
};

class UpdateEvent : public Event
{
public:
	UpdateEvent() {}
};

class FrameStartEvent : public Event
{
public:
	FrameStartEvent() {}
};

class FrameEndEvent : public Event
{
public:
	FrameEndEvent() {}
};

class WindowResizeEvent : public Event
{
public:
	WindowResizeEvent(int _width, int _height, int _frameBufferWidth, int _frameBufferHeight)
		:width(_width), height(_height), frameBufferWidth(_frameBufferWidth), frameBufferHeight(_frameBufferHeight) {}

	const int width;
	const int height;
	const int frameBufferWidth;
	const int frameBufferHeight;
};

class KeyPressEvent : public Event
{
public:
	KeyPressEvent(int _keycode, int _scancode, int _mods)
		:keycode(_keycode), scancode(_scancode), mods(_mods) {}

	const int keycode;
	const int scancode;
	const int mods;
};

class KeyReleaseEvent : public Event
{
public:
	KeyReleaseEvent(int _keycode, int _scancode, int _mods)
		:keycode(_keycode), scancode(_scancode), mods(_mods) {}

	const int keycode;
	const int scancode;
	const int mods;
};

class CharEvent : public Event
{
public:
	CharEvent(unsigned int _codePoint)
		: codePoint (_codePoint) {}

	const int codePoint;
};

class MouseMoveEvent : public Event
{
public:
	MouseMoveEvent(double _xpos, double _ypos)
		:xpos(_xpos), ypos(_ypos) {}

	const double xpos;
	const double ypos;
};

class MousePressEvent : public Event
{
public:
	MousePressEvent(int _button, int _mods)
		:button(_button), mods(_mods) {}

	const int button;
	const int mods;
};

class MouseReleaseEvent : public Event
{
public:
	MouseReleaseEvent(int _button, int _mods)
		: button(_button), mods(_mods) {}

	const int button;
	const int mods;
};

class MouseWheelEvent : public Event
{
public:
	MouseWheelEvent(double _x_offset, double _y_offset)
		: x_offset(_x_offset), y_offset(_y_offset) {}
	
	const double x_offset;
	const double y_offset;
};
