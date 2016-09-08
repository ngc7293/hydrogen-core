#include <cmath>

#ifndef VECTOR2D_H_
#define VECTOR2D_H_

class Vector2d {
private:
	double x_, y_;

public:
	Vector2d(double x, double y);
	~Vector2d();

	// Algebric operations
	Vector2d operator+(Vector2d vec);

	Vector2d operator*(int k);
	Vector2d operator*(double k);

	double operator*(Vector2d vec);

	// Projection of this vector on vector 'vec'
	Vector2d proj(Vector2d vec);

	// Access to members
	void set_x(double x);
	void set_y(double y);

	// Note you could simply use a new constructor...
	// FIXME: Is this even needed?
	void set_components(double x, double y);

	void set_norm(double norm);
	void set_angle(double angle);

	double x() { return x_; }
	double y() { return y_; }

	double norm() { return sqrt(pow(x_, 2) + pow(y_, 2)); }
	double angle() { return atan2(y_, x_); }

#ifdef _DEBUG
	void print();
#endif
};

#endif