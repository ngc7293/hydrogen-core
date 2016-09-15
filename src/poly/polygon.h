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

	// The joints that make up the polygon. A Polygon with a single joint is 
	// considered a circle ; the joint is used to specify the radius.
	std::vector<Vector> joints_;

public:
	// Collision checks
	// Return wether both polygons are colliding. Will abort and return true as soon as one is found.
	static bool collision(Polygon& a, Polygon& b);

	// Return a list of all collision points (intersections) between both polygons. This can take 
	// much longer than the previous function
	static std::vector<Vector> collision_points(Polygon& a, Polygon& b);

	// Finds the minimal distance between two polygons. This is relatively well optimized.
	static float distance(Polygon& a, Polygon& b);

public:	
	Polygon(float x, float y);
	~Polygon();

	// Modifiers
	// Add a joint to the vector
	void add(Vector joint);

	// Move the polygon to position (x,y). To check for possible collisions, consider using 
	// move_until() or collision()
	void move(float x, float y);

	// Rotate the polygon around it's origin (pos_). This does not do any collision checking.
	void rotate(float angle);

	// Scale the polygon by multiplying the distance of joints from the circle by factor.
	// This does not dove any collision checking.
	void scale(float factor);

	// Collision checks : these are shortcuts to the static functions
	bool collision(Polygon& b);
	std::vector<Vector> collision_points(Polygon& b);
	float distance(Polygon& b);

	// Return a coefficiet [0, 1] of Vector motion that the polygon could achieve without 
	// colliding with polygon b.
	float move_until(Vector motion, Polygon& b);

	// Member access
	Vector& pos() { return pos_; }
	float radius() { return radius_; }
	std::vector<Vector>& joints() { return joints_; }

	// Shortcut. Polygons with only a single joint are considered circles.
	bool is_circle() { return (joints_.size() == 1); }

#ifdef _DEBUG
	void render();
#endif
};

} //namespace hc

#endif