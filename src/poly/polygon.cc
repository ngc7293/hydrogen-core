#include "poly/polygon.hh"

#include <algorithm>
#include <iostream>
#include <vector>

#include <cmath>
#include <ctime>

#ifdef _DEBUG
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#endif

#include "util/vector.hh"

#ifdef _DEBUG
#include "poly/segment.hh"
#endif

namespace hc {

std::vector<vecf> Polygon::find_axes(Polygon& a, Polygon& b)
{
	std::vector<vecf> axes;
	vecf axis;

	for (unsigned int i = 1; i < a.joints().size(); i++) {
		axis = (a.joints()[i] - a.joints()[i - 1]).normal().unit();

		// Don't add the axis to the list if an equivalent axis is already there
		if (std::none_of(axes.begin(), axes.end(), [axis](vecf in_list) { return (axis == in_list || axis == -in_list); }))
			axes.push_back(axis);
	}

	// Same as above, for polygon b
	for (unsigned int i = 1; i < b.joints().size(); i++) {
		axis = (b.joints()[i] - b.joints()[i - 1]).normal().unit();
		if (std::none_of(axes.begin(), axes.end(), [axis](vecf in_list) { return (axis == in_list || axis == -in_list); }))
			axes.push_back(axis);
	}

	return axes;
}

bool Polygon::collision(Polygon& a, Polygon& b)
{
	// Immediatly quit if the polygons are not within reach of each other
	if (a.radius_sq() + b.radius_sq() < (a.pos() - b.pos()).length_sq())
		return false;

	// Get all axes to check agaisnt
	std::vector<vecf> axes = find_axes(a, b);

	float maxa, mina; // Projection interval for A
	float maxb, minb; // Projection interval for B
	float proj;		  // Projection of a point an axis

	for (vecf& axis : axes) {
		// Find the projection interval for polygon a
		maxa = mina = (((a.pos()) * axis) / (axis * axis));
		for (unsigned int i = 1; i < a.joints().size(); i++) {
			proj = (((a.joints()[i] + a.pos()) * axis) / (axis * axis));

			mina = (proj < mina ? proj : mina);
			maxa = (proj > maxa ? proj : maxa);
		}

		// Find the proj ection interval for polygon b
		maxb = minb = (((b.pos()) * axis) / (axis * axis));
		for (unsigned int i = 1; i < b.joints().size(); i++) {
			proj = (((b.joints()[i] + b.pos()) * axis) / (axis * axis));

			minb = (proj < minb ? proj : minb);
			maxb = (proj > maxb ? proj : maxb);
		}

		// Immediatly quit if the intervals don't overlap : this means there is no collision
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
	std::vector<vecf> axes = find_axes(a, b);

	vecf motion = vecf(0, 0);
	bool first = true;

	float maxa, mina; // Projection interval for A
	float maxb, minb; // Projection interval for B
	float proj;		  // Projection of a point an axis

	for (vecf& axis : axes) {
		// Find the projection interval for polygon a
		maxa = mina = (((a.pos()) * axis) / (axis * axis));
		for (unsigned int i = 1; i < a.joints().size(); i++) {
			proj = (((a.joints()[i] + a.pos()) * axis) / (axis * axis));

			mina = (proj < mina ? proj : mina);
			maxa = (proj > maxa ? proj : maxa);
		}

		// Find the proj ection interval for polygon b
		maxb = minb = (((b.pos()) * axis) / (axis * axis));
		for (unsigned int i = 1; i < b.joints().size(); i++) {
			proj = (((b.joints()[i] + b.pos()) * axis) / (axis * axis));

			minb = (proj < minb ? proj : minb);
			maxb = (proj > maxb ? proj : maxb);
		}

		// Immediatly quit if the intervals don't overlap : this means there is no collision
		if (mina > maxb || minb > maxa)
			return vecf(0, 0);

		// Find the motion vector, which is along the axis
		//FIXME: explain how this works
		axis = axis * (maxa > maxb ? maxb - mina : minb - maxa);

		// Always pick the smallest motion possible to resolve the collision
		if (first || axis.length_sq() < motion.length_sq()) {
			motion = axis;
			first = false;
		}
	}

	return motion;
}

Polygon::Polygon(vecf pos)
	: pos_(pos)
{
	radius_sq_ = 0;
}

Polygon::~Polygon()
{
}

void Polygon::rotate(float angle)
{
	for (vecf joint : joints_)
		joint.set_angle(joint.angle() + angle);
}

void Polygon::scale(float factor)
{
	for (vecf joint : joints_)
		joint *= factor;
}

void Polygon::add(vecf joint)
{
	// Remove the last joint (which is the first joint)
	if (joints_.size() > 1)
		joints_.pop_back();

	// Add the new joint
	joints_.push_back(joint);

	// Put the first joint back at the end
	if (joints_.size() > 1)
		joints_.push_back(joints_.front());

	// Update the radius_sq_ if needed
	if ((joint.length_sq() + 16) > radius_sq_)
		radius_sq_ = (joint.length_sq() + 16);
}

#ifdef _DEBUG
void Polygon::render()
{
	for (unsigned int i = 1; i < joints_.size(); i++)
		Segment(joints_[i - 1] + pos_, joints_[i] - joints_[i - 1]).render();

	al_draw_circle(pos_.x(), pos_.y(), sqrt(radius_sq_), al_map_rgb(127, 127, 255), 2);
}
#endif

} //namespace hc