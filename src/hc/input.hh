#ifndef INPUT_H_
#define INPUT_H_

#include <allegro5/allegro.h>

#include "hc/defs.hh"

#include "util/vector.hh"

namespace hc {

class Input {
public:
	// Possible States for keyboard keys and mouse buttons.
	// Note : With this logic, a key or button will be set as PRESSED or RELEASED for a full tick,
	// before being set to DOWN or UP. This allows the objects to react to a key press or release,
	// whilst keeping all the code inside the Object::update() function.
	// To ensure this works properly, Manager::update() must be called before Input::update().
	enum State { PRESSED,
		DOWN,
		RELEASED,
		UP };

private:
	// Saves the state of all keyboard keys
	State keyboard_[ALLEGRO_KEY_MAX];

	// Saves the state of the $MOUSE_BUTTON_MAX first mouse buttons
	State mousebutton_[MOUSE_BUTTON_MAX];

	// The mouses axis
	vecf mxy_;
	float mz_;

public:
	Input();
	~Input();

	// React to the event : does nothing if the event isn't an input event
	void handle(ALLEGRO_EVENT& event);

	// Update the State of keys and mouse buttons
	void update();

	// Return true if key 'key' is of state 'stat
	// Note : if you ask for 'DOWN' and key is 'PRESSED', this wil return true. Same for 'RELEASED' */
	bool isKey(State state, int key);

	// Return true if 'button' is of state 'stat
	bool isMouseButton(State state, int button);

	// Return mouse position
	vecf mxy() { return mxy_; }

	// Return the mouse x (relative to the display)
	float mx() { return mxy_.x(); }

	// Return the mouse y (relative to the display)
	float my() { return mxy_.y(); }

	// Return the mouse z (relative to the display)
	float mz() { return mz_; }
	void set_mz(float mz);
};

} //namespace hc

#endif