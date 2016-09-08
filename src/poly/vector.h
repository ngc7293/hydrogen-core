#include <cmath>

#ifndef VECTOR_H_
#define VECTOR_H_

namespace hc {

class Vector {
private:
	double x_, y_;

public:
	Vector(double x, double y);
	~Vector();

	// Algebric operations
	Vector operator+(Vector vec);

	Vector operator*(int k);
	Vector operator*(double k);

	double operator*(Vector vec);

	// Projection of this vector on vector 'vec'
	Vector proj(Vector vec);

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

} //namespace hc

#endif