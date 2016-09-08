#include "vector.h"

#ifndef SEGMENT_H_
#define SEGMENT_H_

namespace hc {

class Segment {
private:
	Vector origin_, direction_;

public:
	static bool intersection(Segment& a, Segment& b);

public:
	Segment(Vector origin, Vector director);
	Segment(double x, double y, Vector director);
	Segment(double x, double y, double x2, double y2);
	~Segment();

	Vector& origin() { return origin_; }
	Vector& direction() { return direction_; }

#ifdef _DEBUG
	void render();
#endif
};

};

#endif