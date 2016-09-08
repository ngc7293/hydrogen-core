#include "boundingpoly.h"

#include <vector>

#include "segment.h"

namespace hc {

BoundingPoly::BoundingPoly(double x, double y)
    : x_(x)
    , y_(y)
{
}

BoundingPoly::BoundingPoly(double x, double y, std::vector<Segment> segments)
    : x_(x)
    , y_(y)
    , segments_(segments)
{
}

BoundingPoly::~BoundingPoly() {}

bool BoundingPoly::collision(BoundingPoly& a, BoundingPoly& b)
{
	for (unsigned int i(0); i < a.segments().size(); i++)
		for (unsigned int j(0); j < b.segments().size(); j++)
			if (Segment::intersection(a.segments()[i], b.segments()[j]))
				;
	return false;
}

void BoundingPoly::add(Segment segment)
{
	segments_.push_back(segment);
}

void BoundingPoly::move(double dx, double dy)
{
	for (unsigned int i(0); i < segments_.size(); i++) {
		//segments_[i].origin() = segments_[i].origin() + Vector(dx, dy);
		segments_[i].origin() += Vector(dx, dy);
	}
}

} //namespace hc