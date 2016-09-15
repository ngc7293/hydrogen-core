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

	Vector operator-();
	Vector operator-(Vector vec);
	void operator-=(Vector vec);

	// Scalar multiplication
	//Vector operator*(int k);
	//void operator*=(int k);
	Vector operator*(float k);
	void operator*=(float k);

	// Dot product
	float operator*(Vector vec);

	// Cross product (for lack of a 'x' operator)
	float operator%(Vector vec);

	// Projection of this vector on vector 'vec'
	Vector proj(Vector vec);

	// Shortcut for checking if vector is null (this == Vector(0, 0))
	bool null() { return (x_ == 0 && y_ == 0); }

	// Same as above. Note : using this function with any number other than 0 will return false
	bool operator==(unsigned int zero);

	// Setting components
	void set_x(float x) { x_ = x; }
	void set_y(float y) { y_ = y; }

	// Setting through geometric values. These have a longer
	// implementation, and can be found in the .cc file
	void set_length(float length);
	void set_angle(float angle);

	// Reading componenents
	float x() { return x_; }
	float y() { return y_; }

	// Reading geometric values
	float length() { return sqrt(pow(x_, 2) + pow(y_, 2)); }
	float length_sq() { return pow(x_, 2) + pow(y_, 2); }
	float angle() { return atan2(y_, x_); }

#ifdef _DEBUG
	void print();
#endif
};

} //namespace hc

#endif