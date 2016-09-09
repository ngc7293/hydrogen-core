#include <vector>

#include "segment.h"

#ifndef BOUNDINGPOLY_H_
#define BOUNDINGPOLY_H_

namespace hc {

class BoundingPoly {
private:
	Vector origin_;
	std::vector<Segment> segments_;

	float radius_;

public:
	static float mindistance(BoundingPoly& a, BoundingPoly& b);
	static bool collision(BoundingPoly& a, BoundingPoly& b);

public:	
	BoundingPoly(float x, float y);
	BoundingPoly(float x, float y, std::vector<Segment> segments);
	~BoundingPoly();

	void add(Segment segment);

	void move(float x, float y);
	void rotate(float angle);

	Vector& origin() { return origin_; }
	std::vector<Segment>& segments() { return segments_; }

	float radius() { return radius_; }

#ifdef _DEBUG
	void render();
#endif
};

} //namespace hc

#endif