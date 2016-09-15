#include "vector.h"

#ifndef SEGMENT_H_
#define SEGMENT_H_

namespace hc {

class Segment {
private:
	// Position of the origin of the segment.
	Vector pos_;

	// Direction vector of the segment. Is also used to set the 
	// length such as the endpoint of the segment is [pos_ + dir_]
	Vector dir_;

public:
	static float distance(Segment a, Segment b);
	static float distance(Segment a, Vector point);

	static bool intersection(Segment a, Segment b);

	// Return the point at which two segment intersect.
	// If the segments don't intersect, returns the Vector(nan, nan)
	static Vector intersection_point(Segment a, Segment b);

public:
	// Default constructor (0, 0, 0, 0)
	Segment();

	// This might need to be changed into origin, endpoint, which is more often needed
	Segment(Vector pos, Vector dir);
	Segment(float x, float y, Vector dir);
	Segment(float x, float y, float x2, float y2);
	~Segment();

	Vector& pos() { return pos_; }
	Vector& dir() { return dir_; }
	Vector end() { return pos_ + dir_; }

#ifdef _DEBUG
	void render();
#endif
};
};

#endif