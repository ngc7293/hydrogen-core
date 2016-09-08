#include "input.h"

namespace hc {

Input::Input()
{
	for (int i(0); i < ALLEGRO_KEY_MAX; i++)
		keyboard_[i] = UP;

	for (int i(0); i < MOUSE_BUTTON_MAX; i++)
		mousebutton_[i] = UP;

	mx_ = 0;
	my_ = 0;
	mz_ = -2;
}

Input::~Input() {}

void Input::handle(ALLEGRO_EVENT& event)
{
	// React to keyboard interaction
	if (event.type == ALLEGRO_EVENT_KEY_DOWN)
		keyboard_[event.keyboard.keycode] = PRESSED;
	else if (event.type == ALLEGRO_EVENT_KEY_UP)
		keyboard_[event.keyboard.keycode] = RELEASED;

	// React to mouse button interaction
	if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
		mousebutton_[event.mouse.button - 1] = PRESSED;
	else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
		mousebutton_[event.mouse.button - 1] = RELEASED;

	// React to the mouse moving about
	if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
		mx_ = event.mouse.x;
		my_ = event.mouse.y;
		mz_ = event.mouse.z;
	}
}

void Input::update()
{
	// If the key was PRESSED or RELEASED last tick, change it to DOWN or UP
	for (int i(0); i < ALLEGRO_KEY_MAX; i++)
		if (keyboard_[i] == PRESSED)
			keyboard_[i] = DOWN;
		else if (keyboard_[i] == RELEASED)
			keyboard_[i] = UP;

	// Same as above, for the mouse buttons
	for (int i(0); i < MOUSE_BUTTON_MAX; i++)
		if (mousebutton_[i] == PRESSED)
			mousebutton_[i] = DOWN;
		else if (mousebutton_[i] == RELEASED)
			mousebutton_[i] = UP;
}

bool Input::isKey(State state, int key)
{
	if (key > ALLEGRO_KEY_MAX)
		return false;

	if (keyboard_[key] == state)
		return true;

	if ((state == DOWN && keyboard_[key] == PRESSED) || (state == UP && keyboard_[key] == RELEASED))
		return true;

	return false;
}

bool Input::isMouseButton(State state, int button)
{
	if (button > MOUSE_BUTTON_MAX)
		return false;

	if (mousebutton_[button] == state)
		return true;

	if ((state == DOWN && mousebutton_[button] == PRESSED) || (state == UP && mousebutton_[button] == RELEASED))
		return true;

	return false;
}

} //namespace hc