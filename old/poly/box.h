#ifndef BOX_H_
#define BOX_H_

#include <vector>

#include "object.h"
#include "segment.h"
#include "vector.h"

namespace hc {

class Box : public Object {
public:
	static const int TYPE_ID = 1;

private:
	// Position
	vecf pos_;

	// Collision data
	float radius_sq_;
	std::vector<vecf> joints_;

	// Is this box controlled by the user
	bool control_;

public:
	static bool collision(Box& a, Box& b);
	static vecf resolve(Box& a, Box& b); // Where a is movable and b is fixed

public:
	Box(vecf pos, bool control = false);
	~Box();
	
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