#include <vector>

#include "segment.h"

#ifndef Polygon_H_
#define Polygon_H_

namespace hc {

class Polygon {
private:
	// x,y coordinates of the center of the Polygon. Joints are set relative 
	// to this coordinate.
	Vector pos_;

	// Maximum distance within which at collision is possible. Used to quickly 
	// determine if a collision is even possible. Calculated by taking the distance 
	// between the furthest joint and pos_
	float radius_;

	// The joints that make up the polygon. A Polygon with a single joint is considered a circle.
	std::vector<Vector> joints_;

public:
	// Collision checks
	static bool collision(Polygon& a, Polygon& b);
	static std::vector<Vector> collision_points(Polygon& a, Polygon& b);

	static float distance(Polygon& a, Polygon& b);

public:	
	Polygon(float x, float y);
	~Polygon();

	// Modifiers
	void add(Vector joint);
	void move(float x, float y);
	void rotate(float angle);
	void scale(float factor);

	// Collision checks : these are shortcuts to the static functions
	bool collision(Polygon& b);
	std::vector<Vector> collision_points(Polygon& b);

	float distance(Polygon& b);
	float move_until(Vector motion, Polygon& b);

	// Member access
	bool is_circle() { return (joints_.size() == 1); }
	Vector& pos() { return pos_; }
	float radius() { return radius_; }
	std::vector<Vector>& joints() { return joints_; }


#ifdef _DEBUG
	void render();
#endif
};

} //namespace hc

#endif