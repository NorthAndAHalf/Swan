#include "Event.h"

KeyPressEvent::KeyPressEvent(int _keycode, int _scancode, int _mods)
	: keycode(_keycode), scancode(_scancode), mods(_mods)
{
}

KeyReleaseEvent::KeyReleaseEvent(int _keycode, int _scancode,  int _mods)
	: keycode(_keycode), scancode(_scancode), mods(_mods)
{
}
