#include "vector.h"

#ifdef _DEBUG
#include <iostream>
#endif

#include <cmath>

namespace hc {

vec2::vec2(float x, float y)
	: x_(x)
	, y_(y)
{
}

vec2::~vec2() {}

bool vec2::operator==(vec2 vec)
{
	return (x_ == vec.x() && y_ == vec.y());
}

vec2 vec2::operator+(vec2 vec)
{
	return vec2(x_ + vec.x(), y_ + vec.y());
}

void vec2::operator+=(vec2 vec)
{
	x_ += vec.x();
	y_ += vec.y();
}

vec2 vec2::operator-()
{
	return vec2(-x_, -y_);
}

vec2 vec2::operator-(vec2 vec)
{
	return vec2(x_ - vec.x(), y_ - vec.y());
}

void vec2::operator-=(vec2 vec)
{
	x_ -= vec.x();
	y_ -= vec.y();
}

vec2 vec2::operator*(float k)
{
	return vec2(k * x_, k * y_);
}

void vec2::operator*=(float k)
{
	x_ *= k;
	y_ *= k;
}

float vec2::operator*(vec2 vec)
{
	return x_ * vec.x() + y_ * vec.y();
}

float vec2::operator%(vec2 vec)
{
	return x_ * vec.y() - y_ * vec.x();
}

vec2 vec2::proj(vec2 vec)
{
	if (vec.length() == 0)
		return vec2(0, 0);

	return vec2(vec.x(), vec.y()) * ((*this * vec) / (vec * vec));
}

void vec2::set_length(float length)
{
	float angle = vec2::angle();
	x_ = length * cos(angle);
	y_ = length * sin(angle);
}

void vec2::set_angle(float angle)
{
	float length = vec2::length();
	x_ = length * cos(angle);
	y_ = length * sin(angle);
}

#ifdef _DEBUG
void vec2::print()
{
	std::cout << "[" << x_ << ", " << y_ << "]" << std::endl;
}
#endif

} //namespace hc