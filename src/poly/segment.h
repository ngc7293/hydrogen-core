#include "vector.h"

#ifndef SEGMENT_H_
#define SEGMENT_H_

namespace hc {

class Segment {
private:
	Vector origin_, direction_;

public:
	static float mindistance(Segment& a, Segment& b);
	static float mindistance(Segment& a, Vector& p);
	static bool intersection(Segment& a, Segment& b);

public:
	Segment(Vector origin, Vector director);
	Segment(float x, float y, Vector director);
	Segment(float x, float y, float x2, float y2);
	~Segment();

	Vector& origin() { return origin_; }
	Vector& direction() { return direction_; }

#ifdef _DEBUG
	void render();
#endif
};

};

#endif