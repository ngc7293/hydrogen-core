#include "vector2d.h"

#ifdef _DEBUG
#include <iostream>
#endif

#include <cmath>

Vector2d::Vector2d(double x, double y)
	: x_(x)
	, y_(y)
{
}

Vector2d::~Vector2d() {}

Vector2d Vector2d::operator+(Vector2d vec)
{
	return Vector2d(x_ + vec.x(), y_ + vec.y());
}

Vector2d Vector2d::operator*(int k)
{
	return Vector2d(k * x_, k * y_);
}

Vector2d Vector2d::operator*(double k)
{
	return Vector2d(k * x_, k * y_);
}

double Vector2d::operator*(Vector2d vec)
{
	return x_ * vec.x() + y_ * vec.y();
}

Vector2d Vector2d::proj(Vector2d vec)
{
	if (vec.norm() == 0)
		return Vector2d(0, 0);

	return Vector2d(vec.x(), vec.y()) * ((*this * vec) / (vec * vec));
}

void Vector2d::set_x(double x)
{
	set_components(x, y_);
}

void Vector2d::set_y(double y)
{
	set_components(x_, y);
}

void Vector2d::set_components(double x, double y)
{
	x_ = x;
	y_ = y;
}

void Vector2d::set_norm(double norm)
{
	double angle = Vector2d::angle();
	x_ = norm * cos(angle);
	y_ = norm * sin(angle);
}

void Vector2d::set_angle(double angle)
{
	double norm = Vector2d::norm();
	x_ = norm * cos(angle);
	y_ = norm * sin(angle);
}

#ifdef _DEBUG
void Vector2d::print()
{
	std::cout << "[" << x_ << ", " << y_ << "]" << std::endl;
}
#endif