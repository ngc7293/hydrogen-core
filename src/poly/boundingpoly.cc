#include "boundingpoly.h"

#include <iostream>
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

float BoundingPoly::mindistance(BoundingPoly& a, BoundingPoly& b)
{
	float min(-1), cur;

	for (unsigned int i(0); i < a.segments().size(); i++) {
		for (unsigned int j(0); j < b.segments().size(); j++) {
			a.segments()[i].origin() += a.origin();
			b.segments()[j].origin() += b.origin();

			cur = Segment::mindistance(a.segments()[i], b.segments()[j]);
			min = (min == -1 ? cur : (min > cur ? cur : min));

			a.segments()[i].origin() -= a.origin();
			b.segments()[j].origin() -= b.origin();
		}
	}

	return min;
}

bool BoundingPoly::collision(BoundingPoly& a, BoundingPoly& b)
{
	// FIXME: It would be best to avoid actually changing the values is possible.
	// This might require making Segment::intersection not use references
	// Would this affect performance? Should that also be avoided?
	for (unsigned int i(0); i < a.segments().size(); i++) {
		for (unsigned int j(0); j < b.segments().size(); j++) {
			a.segments()[i].origin() += a.origin();
			b.segments()[j].origin() += b.origin();
			bool result = Segment::intersection(a.segments()[i], b.segments()[j]);
			a.segments()[i].origin() -= a.origin();
			b.segments()[j].origin() -= b.origin();

			if (result)
				;
			//return true?;
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
	al_draw_filled_circle(origin_.x(), origin_.y(), 3, al_map_rgb(255, 255, 255));
}

} //namespace hc