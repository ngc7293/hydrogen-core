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
	bool operator==(Vector vec);

	Vector operator+(Vector vec);
	void operator+=(Vector vec);

	Vector operator-(Vector vec);
	void operator-=(Vector vec);


	Vector operator*(int k);
	Vector operator*(double k);

	double operator*(Vector vec);
	double operator%(Vector vec);

	// Projection of this vector on vector 'vec'
	Vector proj(Vector vec);

	// Setting components
	void set_x(double x) { x_ = x; }
	void set_y(double y) { y_ = y; }

	// Setting through geometric values. These have a longer
	// implementation, and can be found in the .cc file
	void set_norm(double norm);
	void set_angle(double angle);

	// Reading componenents
	double x() { return x_; }
	double y() { return y_; }

	// Reading geometric values
	double norm() { return sqrt(pow(x_, 2) + pow(y_, 2)); }
	double angle() { return atan2(y_, x_); }

#ifdef _DEBUG
	void print();
#endif
};

} //namespace hc

#endif