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

Segment::Segment(double x, double y, Vector direction)
	: origin_(x, y)
	, direction_(direction)
{
}

Segment::Segment(double x, double y, double x2, double y2)
	: origin_(x, y)
	, direction_(x2 - x, y2 - y)
{
}

Segment::~Segment() {}

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
			double t0 = ((b.origin() - a.origin()) * a.direction()) / (a.direction() * a.direction());
			double t1 = t0 + ((b.direction() * a.direction()) / (a.direction() * a.direction()));

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
		double t = (b.origin() - a.origin()) % b.direction() / (a.direction() % b.direction());
		double u = (b.origin() - a.origin()) % a.direction() / (a.direction() % b.direction());

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

	//al_draw_filled_circle(origin_.x(), origin_.y(), 3, al_map_rgb(0, 255, 0));
	//al_draw_filled_circle(origin_.x() + direction_.x(), origin_.y() + direction_.y(), 3, al_map_rgb(0, 255, 0));
}

} //namespace hc