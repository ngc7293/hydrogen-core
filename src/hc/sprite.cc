#include "sprite.h"

#include <string>

#include <allegro5/allegro.h>

#include "game.h"
#include "media.h"
#include "object.h"
#include "vector.h"
#include "view.h"

namespace hc {

Sprite::Sprite(std::string file, int framecount, int animspeed, Sprite* successor, Sprite** owner)
{
	// Bitmap
	bitmap_ = hc::Game::game().media().image(file);
	width_ = al_get_bitmap_width(bitmap_);
	height_ = al_get_bitmap_height(bitmap_);

	// Frames
	frame_ = 0;
	split(framecount);

	// Animation
	animspeed_ = animspeed;
	animstep_ = 0;

	// Succession
	successor_ = successor;
	owner_ = owner;
}

Sprite::~Sprite()
{
	hc::Game::game().media().unref(bitmap_);
}

//FIXME: This logic, combined with the draw logic seems to create strange offsets in
// drawn bitmaps (bitmaps drawn at the wrong position)
void Sprite::split(int framecount)
{
	framecount_ = framecount;
	framewidth_ = width_ / framecount_;
}

void Sprite::draw(vecf pos)
{
	draw(pos, 0, 1, 1);
}

void Sprite::draw(vecf pos, float angle)
{
	draw(pos, angle, 1, 1);
}

void Sprite::draw(vecf pos, float angle, float xscale, float yscale)
{
	drawFrame(pos, frame_, angle, xscale, yscale);

	// Animate
	animstep_++;
	if (animstep_ == animspeed_) {
		animstep_ = 0;
		frame_++;

		if (frame_ == framecount_) {
			if (successor_ && owner_)
				*owner_ = successor_;
			frame_ = 0;
		}
	}
}

void Sprite::drawFrame(vecf pos, int frame)
{
	drawFrame(pos, frame, 0, 1, 1);
}

void Sprite::drawFrame(vecf pos, int frame, float angle)
{
	drawFrame(pos, frame, angle, 1, 1);
}

void Sprite::drawFrame(vecf pos, int frame, float angle, float xscale, float yscale)
{
	hc::View& view = hc::Game::game().view();
	pos = hc::View::onDisplay(pos);
	al_draw_tinted_scaled_rotated_bitmap_region(bitmap_, frame * framewidth_, 0, framewidth_, height_,
		al_map_rgba(255, 255, 255, 255), framewidth_ / 2.0, height_ / 2.0, pos.x(), pos.y(),
		xscale * view.scale() * 0.5, yscale * view.scale() * 0.5, angle, 0);
}

void Sprite::set_frame(int frame)
{
	frame_ = frame;
	animstep_ = 0;
}

void Sprite::set_succession(Sprite* successor, Sprite** owner)
{
	successor_ = successor;
	owner_ = owner;
}

} // namespace hc