#ifndef SPRITE_H_
#define SPRITE_H_

#include <string>

#include <allegro5/allegro.h>

#include "util/vector.hh"

namespace hc {

class Sprite {
private:
	// Bitmap
	ALLEGRO_BITMAP* bitmap_;
	int width_, height_;

	// Animation
	int frame_, framecount_, framewidth_;

	// The speed at which to animate, in steps per frame
	int animspeed_, animstep_;

	// The sprite to point owner too after animation is over. Usefuls for
	// non-looping animations. This does not delete anything. If any of
	// these are set to nullptr (as is by default), nothing happens.
	Sprite* successor_;
	Sprite** owner_;

public:
	Sprite(std::string file, int framecount = 1, int animspeed = 1, Sprite* successor = nullptr, Sprite** owner = nullptr);
	~Sprite();

	// Transform a sprite strip into an animated sprite
	void split(int framecount);

	// Draw current frame
	void draw(vecf pos);
	void draw(vecf pos, float angle);
	void draw(vecf pos, float angle, float xscale, float yscale);

	// Draw specific frame
	void drawFrame(vecf pos, int frame);
	void drawFrame(vecf pos, int frame, float angle);
	void drawFrame(vecf pos, int frame, float angle, float xscale, float yscale);

	// Access to members
	// Return current frame
	int frame() const { return frame_; }
	void set_frame(int frame);

	// Return animation speed (in steps per frame)
	int animspeed() const { return animspeed_; }
	void set_animspeed(int animspeed) { animspeed_ = animspeed; }

	// Return sprite to point owner to when animation is over.
	Sprite* successor() const { return successor_; }
	void set_successor(Sprite* successor) { successor_ = successor; }
	void set_succession(Sprite* successor, Sprite** owner);

	// Return owning pointer
	Sprite** owner() const { return owner_; }
	void set_owner(Sprite** owner) { owner_ = owner; }
};

} // namespace hc

#endif