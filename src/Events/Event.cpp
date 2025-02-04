#include "Event.h"

KeyPressEvent::KeyPressEvent(int _keycode, int _scancode, int _mods)
	: keycode(_keycode), scancode(_scancode), mods(_mods)
{
}

KeyReleaseEvent::KeyReleaseEvent(int _keycode, int _scancode,  int _mods)
	: keycode(_keycode), scancode(_scancode), mods(_mods)
{
}

MouseMoveEvent::MouseMoveEvent(double _xpos, double _ypos)
	: xpos(_xpos), ypos(_ypos)
{
}

MousePressEvent::MousePressEvent(int _button, int _mods)
	: button(_button), mods(_mods)
{
}

MouseReleaseEvent::MouseReleaseEvent(int _button, int _mods)
	: button(_button), mods(_mods)
{
}
