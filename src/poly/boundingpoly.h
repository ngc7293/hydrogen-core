#include <vector>

#include "segment.h"

#ifndef BOUNDINGPOLY_H_
#define BOUNDINGPOLY_H_

namespace hc {

class BoundingPoly {
private:
	Vector pos_;
	std::vector<Vector> joints_;

	float radius_;

public:
	static float mindistance(BoundingPoly& a, BoundingPoly& b);
	static bool collision(BoundingPoly& a, BoundingPoly& b);
	static std::vector<Vector> collision_points(BoundingPoly& a, BoundingPoly& b);

public:	
	BoundingPoly(float x, float y);
	~BoundingPoly();

	void add_joint(Vector joint);

	void move(float x, float y);
	void rotate(float angle);

	Vector& pos() { return pos_; }
	std::vector<Vector>& joints() { return joints_; }

	float radius() { return radius_; }

#ifdef _DEBUG
	void render();
#endif
};

} //namespace hc

#endif