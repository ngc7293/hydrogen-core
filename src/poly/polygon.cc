#include "polygon.h"

#include <algorithm>
#include <iostream>
#include <vector>

#include <cmath>
#include <ctime>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "game.h"
#include "input.h"
#include "object.h"
#include "vector.h"

namespace hc {

bool Polygon::collision(Polygon& a, Polygon& b)
{
	// v1 = 421 clocks

	// Find all axes to test agaisnt
	std::vector<vecf> axes;
	vecf ax;

	for (unsigned int i = 1; i < a.joints().size(); i++) {
		ax = (a.joints()[i] - a.joints()[i - 1]).normal().unit();
		if (std::none_of(axes.begin(), axes.end(), [ax](vecf axis) { return (axis == ax || axis == -ax); }))
			axes.push_back(ax);
	}

	for (unsigned int i = 1; i < b.joints().size(); i++) {
		ax = (b.joints()[i] - b.joints()[i - 1]).normal().unit();
		if (std::none_of(axes.begin(), axes.end(), [ax](vecf axis) { return (axis == ax || axis == -ax); }))
			axes.push_back(ax);
	}

	float maxa, mina, maxb, minb, x;
	for (vecf& axis : axes) {
		maxa = mina = (((a.pos()) * axis) / (axis * axis));

		for (unsigned int i = 1; i < a.joints().size(); i++) {
			x = (((a.joints()[i] + a.pos()) * axis) / (axis * axis));

			mina = (x < mina ? x : mina);
			maxa = (x > maxa ? x : maxa);
		}

		maxb = minb = (((b.pos()) * axis) / (axis * axis));

		for (unsigned int i = 1; i < b.joints().size(); i++) {
			x = (((b.joints()[i] + b.pos()) * axis) / (axis * axis));

			minb = (x < minb ? x : minb);
			maxb = (x > maxb ? x : maxb);
		}

		if (mina > maxb || minb > maxa)
			return false;
	}
	return true;
}

vecf Polygon::resolve(Polygon& a, Polygon& b)
{
	// If both objects are not within reach of each other, quit
	if (a.radius_sq() + b.radius_sq() < (a.pos() - b.pos()).length_sq())
		return vecf(0, 0);

	// Find all axes to test agaisnt
	// The axes are all the normals to the sides of the polygon
	std::vector<vecf> axes;
	vecf axis;

	for (unsigned int i = 1; i < a.joints().size(); i++) {
		axis = (a.joints()[i] - a.joints()[i - 1]).normal().unit();

		// 
		if (std::none_of(axes.begin(), axes.end(), [axis](vecf in_list) { return (axis == in_list || axis == -in_list); }))
			axes.push_back(axis);
	}

	for (unsigned int i = 1; i < b.joints().size(); i++) {
		axis = (b.joints()[i] - b.joints()[i - 1]).normal().unit();
		if (std::none_of(axes.begin(), axes.end(), [axis](vecf in_list) { return (axis == in_list || axis == -in_list); }))
			axes.push_back(axis);
	}
	
	vecf mov = vecf(0,0);
	bool first = true;
	float maxa, mina, maxb, minb, x;
	for (vecf& axis : axes) {
		maxa = mina = (((a.pos()) * axis) / (axis * axis));
		for (unsigned int i = 1; i < a.joints().size(); i++) {
			x = (((a.joints()[i] + a.pos()) * axis) / (axis * axis));

			mina = (x < mina ? x : mina);
			maxa = (x > maxa ? x : maxa);
		}

		maxb = minb = (((b.pos()) * axis) / (axis * axis));
		for (unsigned int i = 1; i < b.joints().size(); i++) {
			x = (((b.joints()[i] + b.pos()) * axis) / (axis * axis));

			minb = (x < minb ? x : minb);
			maxb = (x > maxb ? x : maxb);
		}

		if (mina > maxb || minb > maxa)
			return vecf(0,0);

		if (maxa > maxb)
			x = maxb - mina;
		else
			x= minb - maxa;

		axis = axis * x;
		if (first || axis.length() < mov.length()) {
			mov = axis;
			first = false;
		}
	}

	return mov;
}

Polygon::Polygon(vecf pos)
	: Object(Polygon::TYPE_ID)
	, pos_(pos)
	, joints_()
{
	radius_sq_ = 0;
}

Polygon::~Polygon()
{
}

void Polygon::add(vecf joint)
{
	if (joints_.size() > 1)
		joints_.pop_back();

	joints_.push_back(joint);

	if (joints_.size() > 1)
		joints_.push_back(joints_.front());

	if ((joint.length_sq()*2) > radius_sq_)
		radius_sq_ = (joint.length_sq() * 2);	
}

void Polygon::rotate(float angle)
{
	for (unsigned int i = 0; i < joints_.size(); i++) {
		joints_[i].set_angle(joints_[i].angle() + angle);
	}
}

void Polygon::update()
{
}

#ifdef _DEBUG
void Polygon::render()
{
	for (unsigned int i = 1; i < joints_.size(); i++)
		Segment(joints_[i - 1] + pos_, joints_[i] - joints_[i - 1]).render();

	al_draw_circle(pos_.x(), pos_.y(), sqrt(radius_sq_), al_map_rgb(127,127,255), 2);
}
#endif

} //namespace hc