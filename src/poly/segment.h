#include "vector.h"

#ifndef SEGMENT_H_
#define SEGMENT_H_

namespace hc {

class Segment {
private:
	// Position of the origin of the segment.
	vecf pos_;

	// Direction vector of the segment. Is also used to set the 
	// length such as the endpoint of the segment is [pos_ + dir_]
	vecf dir_;

public:
	static float distance(Segment a, Segment b);
	static float distance(Segment a, vecf point);

	static bool intersection(Segment a, Segment b);

	// Return the point at which two segment intersect.
	// If the segments don't intersect, returns the vecf(nan, nan)
	static vecf intersection_point(Segment a, Segment b);

public:
	// Default constructor (0, 0, 0, 0)
	Segment();

	// This might need to be changed into origin, endpoint, which is more often needed
	Segment(vecf pos, vecf dir);
	Segment(float x, float y, vecf dir);
	Segment(float x, float y, float x2, float y2);
	~Segment();

	vecf& pos() { return pos_; }
	vecf& dir() { return dir_; }
	vecf end() { return pos_ + dir_; }

#ifdef _DEBUG
	void render();
#endif
};

} //namespace hc

#endif