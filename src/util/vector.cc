#include "vector.h"

#include <cmath>

template <typename T>
bool Vector<T>::USE_PERMISSIVE_EQUALITY = false;

template <typename T>
Vector<T>::Vector(T x, T y)
	: x_(x)
	, y_(y)
{
}

//template <typename T>
//Vector<T>::~Vector() {}

template <typename T>
Vector<T> Vector<T>::operator+(const Vector<T>& vec) const
{
	return Vector<T>(x_ + vec.x(), y_ + vec.y());
}

template <typename T>
Vector<T>& Vector<T>::operator+=(const Vector<T>& vec)
{
	x_ += vec.x();
	y_ += vec.y();
	return *this;
}

template <typename T>
Vector<T> Vector<T>::operator-() const
{
	return Vector<T>(-x_, -y_);
}

template <typename T>
Vector<T> Vector<T>::operator-(const Vector<T>& vec) const
{
	return Vector<T>(x_ - vec.x(), y_ - vec.y());
}

template <typename T>
Vector<T>& Vector<T>::operator-=(const Vector<T>& vec)
{
	x_ -= vec.x();
	y_ -= vec.y();
	return *this;
}

template <typename T>
Vector<T> Vector<T>::operator*(const T& k) const
{
	return Vector<T>(k * x_, k * y_);
}

template <typename T>
Vector<T>& Vector<T>::operator*=(const T& k)
{
	x_ *= k;
	y_ *= k;
	return *this;
}

template <typename T>
Vector<T> Vector<T>::operator/(const T& k) const
{
	return *this * (1 / k);
}

template <typename T>
Vector<T>& Vector<T>::operator/=(const T& k)
{
	*this *= 1 / k;
	return *this;
}

template <typename T>
T Vector<T>::operator*(const Vector<T>& vec) const
{
	return x_ * vec.x() + y_ * vec.y();
}

template <typename T>
T Vector<T>::operator%(const Vector<T>& vec) const
{
	return x_ * vec.y() - y_ * vec.x();
}
template <typename T>
bool Vector<T>::operator==(const Vector<T>& vec) const
{
	if (!USE_PERMISSIVE_EQUALITY)
		return (x_ == vec.x() && y_ == vec.y());
	else
		return ((*this - vec).length_sq() < 0.000001);
}

template <typename T>
bool Vector<T>::operator==(const T& zero) const
{
	return (zero == 0 ? (x_ == 0 && y_ == 0) : false);
}

template <typename T>
Vector<T> Vector<T>::project(const Vector<T>& vec) const
{
	return (vec == 0 ? Vector<T>(0, 0) : Vector<T>(vec.x(), vec.y()) * ((*this * vec) / (vec * vec)));
}

template <typename T>
Vector<T> Vector<T>::normal() const
{
	return Vector<T>(y_, -x_);
}

template <typename T>
Vector<T> Vector<T>::unit() const 
{
	Vector<T> u = *this;
	u.set_length(1);
	return u;
}

template <typename T>
void Vector<T>::set_length(T length)
{
	T angle = Vector::angle();
	x_ = length * cos(angle);
	y_ = length * sin(angle);
}

template <typename T>
void Vector<T>::set_angle(T angle)
{
	T length = Vector::length();
	x_ = length * cos(angle);
	y_ = length * sin(angle);
}

template <typename T>
T Vector<T>::length_sq() const
{
	return pow(x_, 2) + pow(y_, 2);
}

template <typename T>
T Vector<T>::length() const
{
	return sqrt(length_sq());
}

template <typename T>
T Vector<T>::angle() const
{
	return atan2(y_, x_);
}

// Explicit instantiations
template class Vector<float>;
template class Vector<double>;
template class Vector<long double>;

// Printing
template <typename T>
std::ostream& operator<<(std::ostream& out, const Vector<T>& vec)
{
	return out << "[" << vec.x_ << " " << vec.y_ << "]";
}

// Explicit instantiations
template std::ostream& operator<<<float>(std::ostream& out, const Vector<float>& vec);
template std::ostream& operator<<<double>(std::ostream& out, const Vector<double>& vec);
template std::ostream& operator<<<long double>(std::ostream& out, const Vector<long double>& vec);

// Absolute
template <typename T>
Vector<T> abs(Vector<T> vec)
{
	return Vector<T>(fabs(vec.x()), fabs(vec.y()));
}

// Explicit instantiations
template Vector<float> abs(Vector<float> vec);
template Vector<double> abs(Vector<double> vec);
template Vector<long double> abs(Vector<long double> vec);