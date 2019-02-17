#include "poly/trig.hh"

#include <cmath>

#include "util/vector.hh"

namespace trig {

float distance(vecf& pos, vecf& pos2)
{
	return sqrt(distance_sq(pos, pos2));
}

float distance_sq(vecf& pos, vecf& pos2)
{
	return distance_sq(pos.x(), pos.y(), pos2.x(), pos2.y());
}

float distance(float x, float y, float x2, float y2)
{
	return sqrt(distance_sq(x, y, x2, y2));
}

float distance_sq(float x, float y, float x2, float y2)
{
	return pow(x - x2, 2) + pow(y - y2, 2);
}

} //namespace trig