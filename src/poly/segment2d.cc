#include "segment2d.h"

#ifdef _DEBUG
#include <iostream>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#endif

Segment2d::Segment2d(Vector2d origin, Vector2d direction)
	: origin_(origin)
	, direction_(direction)
{
}

Segment2d::Segment2d(double x, double y, Vector2d direction)
	: origin_(x, y)
	, direction_(direction)
{
}

Segment2d::Segment2d(double x, double y, double x2, double y2)
	: origin_(x, y)
	, direction_(x2 - x, y2 - y)
{
}

Segment2d::~Segment2d() {}

Vector2d Segment2d::intersection(Segment2d a, Segment2d b)
{
	double a_slope = a.direction().y() / a.direction().x();
	double a_intercept = a.origin().y() - a_slope * a.origin().x();

	double b_slope = b.direction().y() / b.direction().x();
	double b_intercept = b.origin().y() - b_slope * b.origin().x();

	double x = (b_intercept - a_intercept) / (a_slope - b_slope);

	// Check if x is within a
	double min = (a.direction().x() > 0 ? a.origin().x() : a.origin().x() + a.direction().x());
	double max = (a.direction().x() > 0 ? a.origin().x() + a.direction().x() : a.origin().x());

	if (x < min || x > max)
		return Vector2d(0, 0);

	// Check if x is within b
	min = (b.direction().x() > 0 ? b.origin().x() : b.origin().x() + b.direction().x());
	max = (b.direction().x() > 0 ? b.origin().x() + b.direction().x() : b.origin().x());
	
	if (x < min || x > max)
		return Vector2d(0, 0);

	return Vector2d(x, a_slope * x + a_intercept);
}

void Segment2d::render()
{
	al_draw_line(origin_.x(), origin_.y(), origin_.x() + direction_.x(), origin_.y() + direction_.y(), al_map_rgb(255, 0, 255), 0);

	al_draw_filled_circle(origin_.x(), origin_.y(), 3, al_map_rgb(0, 255, 0));
	al_draw_filled_circle(origin_.x() + direction_.x(), origin_.y() + direction_.y(), 3, al_map_rgb(0, 255, 0));
}