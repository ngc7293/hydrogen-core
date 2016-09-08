#include <vector>

#include "segment.h"

#ifndef BOUNDINGPOLY_H_
#define BOUNDINGPOLY_H_

namespace hc {

class BoundingPoly {
private:
	double x_, y_;
	std::vector<Segment> segments_;

public:
	static bool collision(BoundingPoly& a, BoundingPoly& b);

public:	
	BoundingPoly(double x, double y);
	BoundingPoly(double x, double y, std::vector<Segment> segments);
	~BoundingPoly();

	void add(Segment segment);

	void move(double dx, double dy);

	std::vector<Segment>& segments() { return segments_; }

};

} //namespace hc

#endif