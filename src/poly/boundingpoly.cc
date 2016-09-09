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
	if ((pow(a.origin().x() - b.origin().x(),2) + pow(a.origin().y() - b.origin().y(),2)) > pow(a.radius() + b.radius(), 2))
		return false;

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
	radius_ = (radius_ > segment.origin().norm() ? radius_ : segment.origin().norm());
	radius_ = (radius_ > Vector(segment.origin() + segment.direction()).norm() ? radius_ : Vector(segment.origin() + segment.direction()).norm());
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