#include "trig.h"

#include <cmath>

#include "vector.h"

float distance(hc::Vector pos, hc::Vector pos2)
{
	return sqrt(distance_sq(pos, pos2));
}

float distance_sq(hc::Vector pos, hc::Vector pos2)
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