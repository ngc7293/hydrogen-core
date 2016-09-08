#include <cmath>

#ifndef VECTOR_H_
#define VECTOR_H_

namespace hc {

class Vector {
private:
	float x_, y_;

public:
	Vector(float x, float y);
	~Vector();

	// Algebric operations
	bool operator==(Vector vec);

	Vector operator+(Vector vec);
	void operator+=(Vector vec);

	Vector operator-(Vector vec);
	void operator-=(Vector vec);


	Vector operator*(int k);
	Vector operator*(float k);

	float operator*(Vector vec);
	float operator%(Vector vec);

	// Projection of this vector on vector 'vec'
	Vector proj(Vector vec);

	// Setting components
	void set_x(float x) { x_ = x; }
	void set_y(float y) { y_ = y; }

	// Setting through geometric values. These have a longer
	// implementation, and can be found in the .cc file
	void set_norm(float norm);
	void set_angle(float angle);

	// Reading componenents
	float x() { return x_; }
	float y() { return y_; }

	// Reading geometric values
	float norm() { return sqrt(pow(x_, 2) + pow(y_, 2)); }
	float angle() { return atan2(y_, x_); }

#ifdef _DEBUG
	void print();
#endif
};

} //namespace hc

#endif