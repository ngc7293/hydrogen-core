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
	Segment from_a, from_b;
	Segment ab(a.origin().x(), a.origin().y(), b.origin().x(), b.origin().y());

	// Note: This logic only works if both BoundingPoly are convex. 
	// If either are concave, this might return an incorrect value.
	for (unsigned int i(0); i < a.segments().size(); i++) {
		a.segments()[i].origin() += a.origin();
		if (Segment::intersection(a.segments()[i], ab)) {
			from_a = a.segments()[i];
			a.segments()[i].origin() -= a.origin();
			break;
		}
		a.segments()[i].origin() -= a.origin();
	}

	for (unsigned int i(0); i < b.segments().size(); i++) {
		b.segments()[i].origin() += b.origin();
		if (Segment::intersection(b.segments()[i], ab)) {
			from_b = b.segments()[i];
			b.segments()[i].origin() -= b.origin();
			break;
		}
		b.segments()[i].origin() -= b.origin();
	}

	return Segment::mindistance(from_a, from_b);
}

bool BoundingPoly::collision(BoundingPoly& a, BoundingPoly& b)
{
	// Compare squared values, because sqrt() is very expensive
	if (distance_sq(a.origin(), b.origin()) > pow(a.radius() + b.radius(), 2))
		return false;

	// FIXME: It would be best to avoid actually changing the values is possible.
	// This might require making Segment::intersection not use references
	// Would this affect performance? Should that also be avoided?
	// FIXME: This checks all collisions. Maybe create a different function that stops 
	// as soon as it finds one.

	bool result(false);
	for (unsigned int i(0); i < a.segments().size(); i++) {
		for (unsigned int j(0); j < b.segments().size(); j++) {
			a.segments()[i].origin() += a.origin();
			b.segments()[j].origin() += b.origin();
			result = (result ? true : Segment::intersection(a.segments()[i], b.segments()[j]));
			a.segments()[i].origin() -= a.origin();
			b.segments()[j].origin() -= b.origin();

			if (result)
				;
			//return true?;
		}
	}
	return result;
}

void BoundingPoly::add(Segment segment)
{
	segments_.push_back(segment);

	// Take the longest radius
	radius_ = (radius_ > segment.origin().length() ? radius_ : segment.origin().length());

	// Compare squared values, because sqrt() is very expensive
	// This is only needed if the polygon is not closed.
	// FIXME: Is this needed?
	radius_ = (pow(radius_,2) > Vector(segment.origin() + segment.direction()).length_sq() ? radius_ : Vector(segment.origin() + segment.direction()).length());
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

#ifdef _DEBUG
void BoundingPoly::render()
{
	al_draw_circle(origin_.x(), origin_.y(), radius_, al_map_rgb(0, 0, 255), 2);
	for (unsigned int i(0); i < segments_.size(); i++)
		Segment(segments_[i].origin() + origin_, segments_[i].direction()).render();
	al_draw_filled_circle(origin_.x(), origin_.y(), 3, al_map_rgb(255, 255, 255));
}
#endif

} //namespace hc