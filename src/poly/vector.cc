#include "vector.h"

#ifdef _DEBUG
#include <iostream>
#endif

#include <cmath>

namespace hc {

Vector::Vector(float x, float y)
	: x_(x)
	, y_(y)
{
}

Vector::~Vector() {}

bool Vector::operator==(Vector vec)
{
	return (x_ == vec.x() && y_ == vec.y());
}

Vector Vector::operator+(Vector vec)
{
	return Vector(x_ + vec.x(), y_ + vec.y());
}

void Vector::operator+=(Vector vec)
{
	x_ += vec.x();
	y_ += vec.y();
}

Vector Vector::operator-()
{
	return Vector(-x_, -y_);
}

Vector Vector::operator-(Vector vec)
{
	return Vector(x_ - vec.x(), y_ - vec.y());
}

void Vector::operator-=(Vector vec)
{
	x_ -= vec.x();
	y_ -= vec.y();
}

Vector Vector::operator*(float k)
{
	return Vector(k * x_, k * y_);
}

void Vector::operator*=(float k)
{
	x_ *= k;
	y_ *= k;
}

float Vector::operator*(Vector vec)
{
	return x_ * vec.x() + y_ * vec.y();
}

float Vector::operator%(Vector vec)
{
	return x_ * vec.y() - y_ * vec.x();
}

Vector Vector::proj(Vector vec)
{
	if (vec.length() == 0)
		return Vector(0, 0);

	return Vector(vec.x(), vec.y()) * ((*this * vec) / (vec * vec));
}

void Vector::set_length(float length)
{
	float angle = Vector::angle();
	x_ = length * cos(angle);
	y_ = length * sin(angle);
}

void Vector::set_angle(float angle)
{
	float length = Vector::length();
	x_ = length * cos(angle);
	y_ = length * sin(angle);
}

#ifdef _DEBUG
void Vector::print()
{
	std::cout << "[" << x_ << ", " << y_ << "]" << std::endl;
}
#endif

} //namespace hc