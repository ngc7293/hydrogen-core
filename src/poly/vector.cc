#include "vector.h"

#ifdef _DEBUG
#include <iostream>
#endif

#include <cmath>

namespace hc {

Vector::Vector(double x, double y)
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

Vector Vector::operator-(Vector vec)
{
	return Vector(x_ - vec.x(), y_ - vec.y());
}

void Vector::operator-=(Vector vec)
{
	x_ -= vec.x();
	y_ -= vec.y();
}

Vector Vector::operator*(int k)
{
	return Vector(k * x_, k * y_);
}

Vector Vector::operator*(double k)
{
	return Vector(k * x_, k * y_);
}

double Vector::operator*(Vector vec)
{
	return x_ * vec.x() + y_ * vec.y();
}

double Vector::operator%(Vector vec)
{
	return x_ * vec.y() - y_ * vec.x();
}

Vector Vector::proj(Vector vec)
{
	if (vec.norm() == 0)
		return Vector(0, 0);

	return Vector(vec.x(), vec.y()) * ((*this * vec) / (vec * vec));
}

void Vector::set_norm(double norm)
{
	double angle = Vector::angle();
	x_ = norm * cos(angle);
	y_ = norm * sin(angle);
}

void Vector::set_angle(double angle)
{
	double norm = Vector::norm();
	x_ = norm * cos(angle);
	y_ = norm * sin(angle);
}

#ifdef _DEBUG
void Vector::print()
{
	std::cout << "[" << x_ << ", " << y_ << "]" << std::endl;
}
#endif

} //namespace hc