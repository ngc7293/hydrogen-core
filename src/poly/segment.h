#include "vector.h"

#ifndef SEGMENT_H_
#define SEGMENT_H_

namespace hc {

class Segment {
private:
	// Position of the origin of the segment.
	vec2 pos_;

	// Direction vector of the segment. Is also used to set the 
	// length such as the endpoint of the segment is [pos_ + dir_]
	vec2 dir_;

public:
	static float distance(Segment a, Segment b);
	static float distance(Segment a, vec2 point);

	static bool intersection(Segment a, Segment b);

	// Return the point at which two segment intersect.
	// If the segments don't intersect, returns the vec2(nan, nan)
	static vec2 intersection_point(Segment a, Segment b);

public:
	// Default constructor (0, 0, 0, 0)
	Segment();

	// This might need to be changed into origin, endpoint, which is more often needed
	Segment(vec2 pos, vec2 dir);
	Segment(float x, float y, vec2 dir);
	Segment(float x, float y, float x2, float y2);
	~Segment();

	vec2& pos() { return pos_; }
	vec2& dir() { return dir_; }
	vec2 end() { return pos_ + dir_; }

#ifdef _DEBUG
	void render();
#endif
};
};

#endif