#include "poly/segment.hh"

#ifdef _DEBUG
#include <iostream>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#endif

#include <cmath>

#include "poly/trig.hh"

namespace hc {

Segment::Segment()
	: pos_(0, 0)
	, dir_(0, 0)
{
}

Segment::Segment(vecf pos, vecf dir)
	: pos_(pos)
	, dir_(dir)
{
}

Segment::Segment(float x, float y, vecf dir)
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

float Segment::distance(Segment& a, Segment& b)
{
	float cur, min;

	// Try all combinations
	min = distance(a, b.pos());

	cur = distance(a, b.end());
	min = (min > cur ? cur : min);

	cur = distance(b, a.pos());
	min = (min > cur ? cur : min);

	cur = distance(b, a.end());
	min = (min > cur ? cur : min);

	return min;
}

float Segment::distance(Segment& a, vecf& p)
{
	// If segment 'a' is a point (length 0), return the distance(a, p)
	if (a.dir() == vecf(0, 0))
		return trig::distance(a.pos(), p);

	// Find the closest point on line 'a' with the form [ao + t * ad]
	float t = ((p - a.pos()) * a.dir()) / (a.dir() * a.dir());

	// Cap 't' to the interval 0:1, to keep it within the segment
	t = (t < 0 ? 0 : (t > 1 ? 1 : t));

	// Find the distance(ao + t * ad, t)
	float dis = trig::distance(a.pos() + a.dir() * t, p);

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

bool Segment::intersection(Segment& a, Segment& b)
{
	return !std::isnan(intersection_point(a, b).x());
}

vecf Segment::intersection_point(Segment& a, Segment& b)
{
	// Most of the credit for this algorithm goes to Stack Overflow user Gareth Rees.
	// This is essentially a step-step C++ copy of his answer linked below.
	// http://stackoverflow.com/a/565282

	// To keep the comments readable I have shortened 'a.pos' to 'ao' and 'a.dir' to 'ad'.
	// Same goes for 'b.pos' and 'b.dir'.

	// Remember that vecf overloads the % operator as a cross-product operator

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
				return vecf(NAN, NAN);
		} else {
			return vecf(NAN, NAN);
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
			return vecf(NAN, NAN);
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