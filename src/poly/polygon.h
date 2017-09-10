#ifndef POLYGON_H_
#define POLYGON_H_

#include <vector>

#include "vector.h"

namespace hc {

class Polygon {
private:
	// Current position of the polygon, in worldspace
	vecf pos_;

	// Square of the radius of the smallest circle centered on pos_ in which the polygon fits
	// 16 is added to this value to help with collision checking
	float radius_sq_;

	// A list of all the joints that make up this polygon. The last joint is a copy of the first one
	// to simplify for-loops in code. This circularity is implemented in the add() function
	std::vector<vecf> joints_;

private:
	static std::vector<vecf> find_axes(Polygon& a, Polygon& b);

public:
	static bool collision(Polygon& a, Polygon& b);
	static vecf resolve(Polygon& a, Polygon& b); // Where a is movable and b is fixed

public:
	Polygon(vecf pos);
	~Polygon();
	
	void rotate(float angle);
	void scale(float factor);

	void add(vecf vec);
	std::vector<vecf> joints() const { return joints_; }

	vecf pos() const { return pos_; }
	void set_pos(vecf pos) { pos_ = pos; }

	float radius_sq() const { return radius_sq_; }

#ifdef _DEBUG
	void render();
#endif
};

} //namespace hc

#endif