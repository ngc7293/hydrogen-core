#include "segment.h"

#ifdef _DEBUG
#include <iostream>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#endif

#include "trig.h"

namespace hc {

Segment::Segment()
	: pos_(0, 0)
	, dir_(0, 0)
{
}

Segment::Segment(Vector pos, Vector dir)
	: pos_(pos)
	, dir_(dir)
{
}

Segment::Segment(float x, float y, Vector dir)
	: pos_(x, y)
	, dir_(dir)
{
}

Segment::Segment(float x, float y, float x2, float y2)
	: pos_(x, y)
	, dir_(x2 - x, y2 - y)
{
}

Segment::~Segment() {}

float Segment::mindistance(Segment a, Segment b)
{
	float cur, min;

	// Try all combinations
	min = mindistance(a, b.pos());

	cur = mindistance(a, b.end());
	min = (min > cur ? cur : min);

	cur = mindistance(b, a.pos());
	min = (min > cur ? cur : min);

	cur = mindistance(b, a.end());
	min = (min > cur ? cur : min);

	return min;
}

float Segment::mindistance(Segment a, Vector p)
{
	// If segment 'a' is a point (length 0), return the distance(a, p)
	if (a.dir() == Vector(0, 0))
		return distance(a.pos(), p);

	// Find the closest point on line 'a' with the form [ao + t * ad]
	float t = ((p - a.pos()) * a.dir()) / (a.dir() * a.dir());

	// Cap 't' to the interval 0:1, to keep it within the segment
	t = (t < 0 ? 0 : (t > 1 ? 1 : t));

	// Find the distance(ao + t * ad, t)
	float dis = distance(a.pos() + a.dir() * t, p);

#ifdef _DEBUG
	// For visualisation purpose, draw the shortest line between the segment and the point.
	// Only do this for distances under 100px, as to not clutter the screen
	if (dis < 100) {
		al_draw_line((a.pos() + (a.dir() * t)).x(), (a.pos() + (a.dir() * t)).y(),
			p.x(), p.y(), al_premul_rgba(127, 127, 127, 127), 2);
	}
#endif
	return dis;
}

bool Segment::intersection(Segment a, Segment b)
{
	return !std::isnan(intersection_point(a, b).x());
}

Vector Segment::intersection_point(Segment a, Segment b)
{
	// Most of the credit for this algorithm goes to Stack Overflow user Gareth Rees.
	// This is essentially a step-step C++ copy of his answer linked below.
	// http://stackoverflow.com/a/565282

	// To keep the comments readable I have shortened 'a.pos' to 'ao' and 'a.dir' to 'ad'.
	// Same goes for 'b.pos' and 'b.dir'.

	// Remember that hc::Vector overloads the % operator as a cross-product operator

	// if [ad x bd == 0] then both vectors are parallel
	if (a.dir() % b.dir() == 0) {

		// if [(bo - ao) x ad == 0] then both segments are colinear
		if ((b.pos() - a.pos()) % a.dir() == 0) {

			// find the t0, t1 such as [ao + t0 * ad == bo] and [ao + t1 * ad == bo + bd]
			float t0 = ((b.pos() - a.pos()) * a.dir()) / (a.dir() * a.dir());
			float t1 = t0 + ((b.dir() * a.dir()) / (a.dir() * a.dir()));

			// if the interval t0:t1 intersects 0:1, then the segments partially overlap
			if ((t0 >= 0 && t0 <= 1) || (t1 >= 0 && t1 <= 1))
				if (t0 < 0 && t1 > 1)
					return a.pos() + a.dir() * 0.5f;
				else if (t0 > 0 && t1 < 1)
					return b.pos() + b.dir() * 0.5f;
				else if (t0 < 0)
					return (a.pos() + b.end()) * 0.5f;
				else
					return (b.pos() + a.end()) * 0.5f;
			else
				return Vector(NAN, NAN);
		} else {
			return Vector(NAN, NAN);
		}

		// if [ad x bd != 0] then both segment might intersect (they would if they were infinite)
	} else {
		// find the u,t such as [ao + t * ad == bo + t * bd]
		float t = (b.pos() - a.pos()) % b.dir() / (a.dir() % b.dir());
		float u = (b.pos() - a.pos()) % a.dir() / (a.dir() % b.dir());

		// if both u and t are within the [0:1], the intersection is within both segments' intervals
		if (0 <= u && u <= 1 && 0 <= t && t <= 1)
			return a.pos() + a.dir() * t;
		else
			return Vector(NAN, NAN);
	}
}

#ifdef _DEBUG
void Segment::render()
{
	al_draw_line(pos_.x(), pos_.y(), pos_.x() + dir_.x(), pos_.y() + dir_.y(), al_map_rgb(255, 0, 255), 0);

	al_draw_filled_circle(pos_.x(), pos_.y(), 2, al_map_rgb(0, 255, 0));
	al_draw_filled_circle(pos_.x() + dir_.x(), pos_.y() + dir_.y(), 2, al_map_rgb(0, 255, 0));
}
#endif

} //namespace hc