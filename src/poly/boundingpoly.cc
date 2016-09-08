#include "boundingpoly.h"

#include <vector>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "segment.h"

namespace hc {

BoundingPoly::BoundingPoly(float x, float y)
    : origin_(x, y)
{
}

BoundingPoly::BoundingPoly(float x, float y, std::vector<Segment> segments)
    : origin_(x, y)
    , segments_(segments)
{
}

BoundingPoly::~BoundingPoly() {}

bool BoundingPoly::collision(BoundingPoly& a, BoundingPoly& b)
{
	for (unsigned int i(0); i < a.segments().size(); i++) {
		for (unsigned int j(0); j < b.segments().size(); j++) {
			a.segments()[i].origin() += a.origin();
			b.segments()[j].origin() += b.origin();
			if (Segment::intersection(a.segments()[i], b.segments()[j]))
				;
			a.segments()[i].origin() -= a.origin();
			b.segments()[j].origin() -= b.origin();
		}
	}
	return false;
}

void BoundingPoly::add(Segment segment)
{
	segments_.push_back(segment);
}

void BoundingPoly::move(float x, float y)
{
	origin_ = Vector(x, y);
}

void BoundingPoly::rotate(float angle)
{
	for (unsigned int i(0); i < segments_.size(); i++) {
		segments_[i].origin().set_angle(segments_[i].origin().angle() + angle);
		segments_[i].direction().set_angle(segments_[i].direction().angle() + angle);
	}	
}

void BoundingPoly::render()
{
	for (unsigned int i(0); i < segments_.size(); i++)
		Segment(segments_[i].origin() + origin_, segments_[i].direction()).render();
	al_draw_filled_circle(origin_.x(), origin_.y() , 3, al_map_rgb(255, 255, 255));
}

} //namespace hc