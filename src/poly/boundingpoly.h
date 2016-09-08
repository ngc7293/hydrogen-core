#include <vector>

#include "segment.h"

#ifndef BOUNDINGPOLY_H_
#define BOUNDINGPOLY_H_

namespace hc {

class BoundingPoly {
private:
	Vector origin_;
	std::vector<Segment> segments_;

public:
	static float mindistance(BoundingPoly& a, BoundingPoly& b);
	static bool collision(BoundingPoly& a, BoundingPoly& b);

public:	
	BoundingPoly(float x, float y);
	BoundingPoly(float x, float y, std::vector<Segment> segments);
	~BoundingPoly();

	void rotate(float angle);

	void render();

	void add(Segment segment);

	void move(float dx, float dy);

	Vector& origin() { return origin_; }
	std::vector<Segment>& segments() { return segments_; }

};

} //namespace hc

#endif