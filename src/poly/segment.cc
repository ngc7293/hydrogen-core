#include "segment.h"

#ifdef _DEBUG
#include <iostream>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#endif

#include "../hc/defaults.h"

namespace hc {

Segment::Segment(Vector origin, Vector direction)
	: origin_(origin)
	, direction_(direction)
{
}

Segment::Segment(float x, float y, Vector direction)
	: origin_(x, y)
	, direction_(direction)
{
}

Segment::Segment(float x, float y, float x2, float y2)
	: origin_(x, y)
	, direction_(x2 - x, y2 - y)
{
}

Segment::~Segment() {}

float Segment::mindistance(Segment& a, Segment& b)
{
	float cur, min;

	// Try all combinations
	min = mindistance(a, b.origin());

	cur = mindistance(a, b.origin() + b.direction());
	min = (min > cur ? cur : min);

	cur = mindistance(b, a.origin());
	min = (min > cur ? cur : min);

	cur = mindistance(b, a.origin() + a.direction());
	min = (min > cur ? cur : min);

	return min;
}

float Segment::mindistance(Segment& a, Vector p)
{
	// If segment 'a' is a point (length 0), return the distance(a, p)
	if (a.direction() == Vector(0, 0))
		return sqrt(pow(a.origin().x() - p.x(), 2) + pow(a.origin().y() - p.y(), 2));

	// Find the closest point on line 'a' with the form [ao + t * ad]
	float t = ((p - a.origin()) * a.direction()) / (a.direction() * a.direction());

	// Cap 't' to the interval 0:1, to keep it within the segment
	t = (t < 0 ? 0 : (t > 1 ? 1 : t));

	// Find the distance(ao + t * ad, t)
	float dis = sqrt(pow((a.origin() + (a.direction() * t)).x() - p.x(), 2) + pow((a.origin() + (a.direction() * t)).y() - p.y(), 2));

	if (dis < 100) {
		al_draw_line((a.origin() + (a.direction() * t)).x(), (a.origin() + (a.direction() * t)).y(),
			p.x(), p.y(), al_map_rgb(127, 127, 127), 2);
	}
	return dis;
}

bool Segment::intersection(Segment& a, Segment& b)
{
	// Most of the credit for this algorithm goes to Stack Overflow user Gareth Rees.
	// This is essentially a step-step C++ copy of his answer linked below.
	// http://stackoverflow.com/a/565282

	// To keep the comments readable I have shortened 'a.origin' to 'ao' and 'a.direction' to 'ad'.
	// Same goes for 'b.origin' and 'b.direction'.

	// Remember that hc::Vector overloads the % operator as a cross-product operator

	// if [ad x bd == 0] then both vectors are parallel
	if (a.direction() % b.direction() == 0) {

		// if [(bo - ao) x ad == 0] then both segments are colinear
		if ((b.origin() - a.origin()) % a.direction() == 0) {

			// find the t0, t1 such as [ao + t0 * ad == bo] and [ao + t1 * ad == bo + bd]
			float t0 = ((b.origin() - a.origin()) * a.direction()) / (a.direction() * a.direction());
			float t1 = t0 + ((b.direction() * a.direction()) / (a.direction() * a.direction()));

			// if the interval t0:t1 intersects 0:1, then the segments partially overlap
			if ((t0 >= 0 && t0 <= 1) || (t1 >= 0 && t1 <= 1)) {
				if (t0 < 0)
					al_draw_line((b.origin() + b.direction()).x(), (b.origin() + b.direction()).y(),
						a.origin().x(), a.origin().y(),
						al_map_rgb(255, 255, 0), 6);
				else
					al_draw_line((a.origin() + a.direction()).x(), (a.origin() + a.direction()).y(),
						b.origin().x(), b.origin().y(),
						al_map_rgb(255, 255, 0), 6);

				return true;
			} else {
				return false;
			}
		} else {
			return false;
		}

		// if [ad x bd != 0] then both segment might intersect (they would if they were infinite)
	} else {
		// find the u,t such as [ao + t * ad == bo + t * bd]
		float t = (b.origin() - a.origin()) % b.direction() / (a.direction() % b.direction());
		float u = (b.origin() - a.origin()) % a.direction() / (a.direction() % b.direction());

		// if both u and t are within the [0:1], the intersection is within both segments' intervals
		if (0 <= u && u <= 1 && 0 <= t && t <= 1) {
			al_draw_filled_circle((a.origin() + (a.direction() * t)).x(), (a.origin() + (a.direction() * t)).y(), 3, al_map_rgb(255, 255, 0));
			return true;
		} else {
			return false;
		}
	}

	return true;
}

void Segment::render()
{
	al_draw_line(origin_.x(), origin_.y(), origin_.x() + direction_.x(), origin_.y() + direction_.y(), al_map_rgb(255, 0, 255), 0);

	al_draw_filled_circle(origin_.x(), origin_.y(), 3, al_map_rgb(0, 255, 0));
	al_draw_filled_circle(origin_.x() + direction_.x(), origin_.y() + direction_.y(), 3, al_map_rgb(0, 255, 0));
}

} //namespace hc