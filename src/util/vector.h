#ifndef VECTOR_H_
#define VECTOR_H_

#include <iostream>

#define vec2 Vector<float>

template <typename T>
class Vector {
public:
	// If this is enabled, vectors within 1e-6 difference are considered equal.
	// This can help with floating point imprecision 
	// This is set to false by default.
	static bool USE_PERMISSIVE_EQUALITY;

private:
	T x_, y_;

public:
	Vector(T x = 0, T y = 0);
	~Vector() {}

	// Addition
	Vector<T> operator+(const Vector<T>& vec) const;
	Vector<T>& operator+=(const Vector<T>& vec);

	// Subtraction
	Vector<T> operator-() const;
	Vector<T> operator-(const Vector<T>& vec) const;
	Vector<T>& operator-=(const Vector<T>& vec);

	// Multiplication
	Vector<T> operator*(const T& k) const;
	Vector<T>& operator*=(const T& k);

	// Disvision
	Vector<T> operator/(const T& k) const;
	Vector<T>& operator/=(const T& k);

	// Dot product
	T operator*(const Vector<T>& vec) const;

	// Cross product
	T operator%(const Vector<T>& vec) const;

	// Comparison with other vector
	bool operator==(const Vector<T>& vec) const;

	// Comparison with 0. Returns true if this is a null vector.
	// Return false if 'zero' != 0 or if the vector is non-null.
	bool operator==(const T& zero) const;

	// Projection
	Vector<T> project(const Vector<T>& vec) const;

	// Setting components
	void set_x(T x) { x_ = x; }
	void set_y(T y) { y_ = y; }

	// Setting through geometric values. These have a longer
	// implementation, and can be found in the .cc file
	void set_length(T length);
	void set_angle(T angle);

	// Reading components
	T x() const { return x_; }
	T y() const { return y_; }

	// Reading geometric values
	T length_sq() const;
	T length() const;
	T angle() const;

	// Printing
	template <typename U>
	friend std::ostream& operator<<(std::ostream& out, const Vector<U>& vec);
};

// Make both components of the vector positive by calling fabs
template <typename T>
Vector<T> abs(Vector<T> vec);

typedef Vector<float> vecf;
typedef Vector<double> veclf;

#endif