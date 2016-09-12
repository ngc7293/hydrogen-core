#include "polygon.h"

#include <iostream>
#include <vector>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "segment.h"
#include "trig.h"

namespace hc {

bool Polygon::collision(Polygon& a, Polygon& b)
{
	// Compare squared values, because sqrt() is very expensive
	if (distance_sq(a.pos(), b.pos()) > pow(a.radius() + b.radius(), 2))
		return false;

	for (unsigned int i(0); i < a.joints().size() - 1; i++) {
		for (unsigned int j(0); j < b.joints().size() - 1; j++) {
			if (Segment::intersection(
				Segment(a.pos() + a.joints()[i], a.joints()[i + 1] - a.joints()[i]),
				Segment(b.pos() + b.joints()[j], b.joints()[j + 1] - b.joints()[j])))
				return true;
		}
	}
	return false;
}

std::vector<Vector> Polygon::collision_points(Polygon& a, Polygon& b)
{
	std::vector<Vector> points;
	if (distance_sq(a.pos(), b.pos()) > pow(a.radius() + b.radius(), 2))
		return points;

	// FIXME: It would be best to avoid actually changing the values is possible.
	// This might require making Segment::intersection not use references
	// Would this affect performance? Should that also be avoided?
	for (unsigned int i(0); i < a.joints().size() - 1; i++) {
		for (unsigned int j(0); j < b.joints().size() - 1; j++) {
			Vector point = Segment::intersection_point(
				Segment(a.pos() + a.joints()[i], a.joints()[i + 1] - a.joints()[i]),
				Segment(b.pos() + b.joints()[j], b.joints()[j + 1] - b.joints()[j]));

			if (!std::isnan(point.x())) {
				al_draw_filled_circle(point.x(), point.y(), 3, al_map_rgb(255, 255, 0));
				points.push_back(point);
			}
		}
	}

	return points;
}

Polygon::Polygon(float x, float y)
	: pos_(x, y)
{
}

Polygon::~Polygon() {}

float Polygon::distance(Polygon& a, Polygon& b)
{
	Segment from_a, from_b;
	Segment ab(a.pos().x(), a.pos().y(), b.pos().x(), b.pos().y());

	// Note: This logic only works if both Polygon are convex.
	// If either are concave, this might return an incorrect value.
	for (unsigned int i(0); i < a.joints().size() - 1; i++) {
		from_a = Segment(a.pos() + a.joints()[i], (a.joints()[i + 1] - a.joints()[i]));
		if (Segment::intersection(from_a, ab))
			break;
	}

	for (unsigned int i(0); i < b.joints().size() - 1; i++) {
		from_b = Segment(b.pos() + b.joints()[i], b.joints()[i + 1] - b.joints()[i]);
		if (Segment::intersection(from_b, ab))
			break;
	}

	return Segment::mindistance(from_a, from_b);
}

void Polygon::add(Vector joint)
{
	if (joints_.size() > 0)
		joints_.pop_back();

	joints_.push_back(joint);
	joints_.push_back(joints_.front());

	// Take the longest radius
	radius_ = (pow(radius_, 2) > joint.length_sq() ? radius_ : joint.length());
}

void Polygon::move(float x, float y)
{
	pos_ = Vector(x, y);
}

void Polygon::rotate(float angle)
{
	for (unsigned int i(0); i < joints_.size(); i++) {
		joints_[i].set_angle(joints_[i].angle() + angle);
	}
}

bool Polygon::collision(Polygon& b)
{
	return collision(*this, b);
}

std::vector<Vector> Polygon::collision_points(Polygon& b)
{
	return collision_points(*this, b);
}

float Polygon::distance(Polygon& b)
{
	return distance(*this, b);
}

float Polygon::move_until(Vector motion, Polygon& b)
{
	float maxfactor(1);

	if (distance_sq(pos_, b.pos()) > pow(radius_ + b.radius() + motion.length(), 2))
		return maxfactor;

	for (unsigned int i(0); i < joints_.size() - 1; i++) {
		for (unsigned int j(0); j < b.joints().size() - 1; j++) {
			Vector point = Segment::intersection_point(
				Segment(pos_ + joints_[i], motion),
				Segment(b.pos() + b.joints()[j], b.joints()[j + 1] - b.joints()[j]));

			Vector point2 = Segment::intersection_point(
				Segment(b.pos() + b.joints()[j], -motion),
				Segment(pos_ + joints_[i], joints_[i + 1] - joints_[i]));

			float factor;

			if (!std::isnan(point.x())) {
				point -= (pos_ + joints_[i]);
				factor = point.length() / motion.length();
				maxfactor = (factor < maxfactor ? factor : maxfactor);
			}
			if (!std::isnan(point2.x())) {
				point2 -= (b.pos() + b.joints()[j]);
				factor = point2.length() / motion.length();
				maxfactor = (factor < maxfactor ? factor : maxfactor);
			}
		}
	}

	return maxfactor;
}


#ifdef _DEBUG
void Polygon::render()
{
	for (unsigned int i(0); i < joints_.size() - 1; i++)
		Segment(pos_ + joints_[i], (joints_[i + 1] - joints_[i])).render();
	al_draw_filled_circle(pos_.x(), pos_.y(), radius_, al_premul_rgba(0, 0, 255, 32));
	al_draw_filled_circle(pos_.x(), pos_.y(), 3, al_map_rgb(255, 255, 255));
}
#endif

} //namespace hc