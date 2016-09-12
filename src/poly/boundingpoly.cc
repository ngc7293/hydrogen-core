#include "boundingpoly.h"

#include <iostream>
#include <vector>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "../hc/defaults.h"
#include "segment.h"
#include "trig.h"

namespace hc {

BoundingPoly::BoundingPoly(float x, float y)
	: pos_(x, y)
{
}

BoundingPoly::~BoundingPoly() {}

float BoundingPoly::mindistance(BoundingPoly& a, BoundingPoly& b)
{
	Segment from_a, from_b;
	Segment ab(a.pos().x(), a.pos().y(), b.pos().x(), b.pos().y());

	// Note: This logic only works if both BoundingPoly are convex.
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

bool BoundingPoly::collision(BoundingPoly& a, BoundingPoly& b)
{
	// Compare squared values, because sqrt() is very expensive
	if (distance_sq(a.pos(), b.pos()) > pow(a.radius() + b.radius(), 2))
		return false;

	// FIXME: It would be best to avoid actually changing the values is possible.
	// This might require making Segment::intersection not use references
	// Would this affect performance? Should that also be avoided?
	bool result(false);
	for (unsigned int i(0); i < a.joints().size() - 1; i++) {
		for (unsigned int j(0); j < b.joints().size() - 1; j++) {
			result = Segment::intersection(
				Segment(a.pos() + a.joints()[i], a.joints()[i + 1] - a.joints()[i]),
				Segment(b.pos() + b.joints()[j], b.joints()[j + 1] - b.joints()[j]));
			if (result)
				return true;
		}
	}
	return result;
}

std::vector<Vector> BoundingPoly::collision_points(BoundingPoly& a, BoundingPoly& b)
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

			if (point.x() != NAN) {
				al_draw_filled_circle(point.x(), point.y(), 3, al_map_rgb(255, 255, 0));
				points.push_back(point);
			}
		}
	}

	return points;
}

void BoundingPoly::add_joint(Vector joint)
{
	if (joints_.size() > 0) {
		joints_.pop_back();
		joints_.push_back(joint);
		joints_.push_back(joints_.front());
	} else {
		joints_.push_back(joint);
		joints_.push_back(joints_.front());
	}

	// Take the longest radius
	radius_ = (pow(radius_, 2) > joint.length_sq() ? radius_ : joint.length());
}

void BoundingPoly::move(float x, float y)
{
	pos_ = Vector(x, y);
}

float BoundingPoly::move_predict(Vector motion, BoundingPoly& b)
{
	float maxfactor(1);

	if (distance_sq(pos_, b.pos()) > pow(radius_ + b.radius() + motion.length(), 2))
		return maxfactor;

	for (unsigned int i(0); i < joints_.size() - 1; i++) {
		for (unsigned int j(0); j < b.joints().size() - 1; j++) {
			Vector point = Segment::intersection_point(
				Segment(pos_ + joints_[i], motion),
				Segment(b.pos() + b.joints()[j], b.joints()[j + 1] - b.joints()[j]));
			al_draw_filled_circle(point.x(), point.y(), 5, al_map_rgb(255, 0, 0));

			if (std::isnan(point.x()))
				continue;

			point -= (pos_ + joints_[i]);

			float factor = point.length() / motion.length();
			maxfactor = (factor < maxfactor ? factor : maxfactor);
		}
	}

	motion.set_angle(motion.angle() + PI);
	for (unsigned int i(0); i < joints_.size() - 1; i++) {
		for (unsigned int j(0); j < b.joints().size() - 1; j++) {
			Segment(b.pos() + b.joints()[j], motion).render();
			Vector point = Segment::intersection_point(
				Segment(b.pos() + b.joints()[j], motion),
				Segment(pos_ + joints_[i], joints_[i + 1] - joints_[i]));
			al_draw_filled_circle(point.x(), point.y(), 5, al_map_rgb(255, 0, 0));

			if (std::isnan(point.x()))
				continue;

			point -= (b.pos() + b.joints()[j]);

			float factor = point.length() / motion.length();
			std::cout << factor << "\n";
			maxfactor = (factor < maxfactor ? factor : maxfactor);
		}
	}

	return maxfactor;
}

void BoundingPoly::rotate(float angle)
{
	for (unsigned int i(0); i < joints_.size(); i++) {
		joints_[i].set_angle(joints_[i].angle() + angle);
	}
}

#ifdef _DEBUG
void BoundingPoly::render()
{
	for (unsigned int i(0); i < joints_.size() - 1; i++)
		Segment(pos_ + joints_[i], (joints_[i + 1] - joints_[i])).render();
	al_draw_filled_circle(pos_.x(), pos_.y(), radius_, al_premul_rgba(0, 0, 255, 32));
	al_draw_filled_circle(pos_.x(), pos_.y(), 3, al_map_rgb(255, 255, 255));
}
#endif

} //namespace hc