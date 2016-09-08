#include "vector2d.h"

#ifndef SEGMENT2D_H_
#define SEGMENT2D_H_

class Segment2d {
private:
	Vector2d origin_, direction_;

public:
	static Vector2d intersection(Segment2d a, Segment2d b);

public:
	Segment2d(Vector2d origin, Vector2d director);
	Segment2d(double x, double y, Vector2d director);
	Segment2d(double x, double y, double x2, double y2);
	~Segment2d();

	Vector2d& origin() { return origin_; }
	Vector2d& direction() { return direction_; }

#ifdef _DEBUG
	void render();
#endif
};

#endif