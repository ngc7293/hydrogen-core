#ifndef POLYGON_H_
#define POLYGON_H_

#include <vector>

#include "object.h"
#include "segment.h"
#include "vector.h"

namespace hc {

class Polygon : public Object {
public:
	static const int TYPE_ID = 1;

private:
	// Position
	vecf pos_;

	// Collision data
	float radius_sq_;
	std::vector<vecf> joints_;

public:
	static bool collision(Polygon& a, Polygon& b);
	static vecf resolve(Polygon& a, Polygon& b); // Where a is movable and b is fixed

public:
	Polygon(vecf pos);
	~Polygon();
	
	void add(vecf vec);
	void rotate(float angle);

	virtual void update();
	virtual void render();

	std::vector<vecf> joints() const { return joints_; }
	vecf pos() const { return pos_; }
	float radius_sq() const { return radius_sq_; }

	void set_pos(vecf pos) { pos_ = pos; }
};

} //namespace hc

#endif