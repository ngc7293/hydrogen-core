#include "segment2d.h"

#ifdef _DEBUG
#include <iostream>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#endif

Segment2d::Segment2d(Vector2d origin, Vector2d director)
	: origin_(origin)
	, director_(director)
{
}

Segment2d::Segment2d(double x, double y, Vector2d director)
	: origin_(x, y)
	, director_(director)
{
}

Segment2d::Segment2d(double x, double y, double x2, double y2)
	: origin_(x, y)
	, director_(x2 - x, y2 - y)
{
}

Segment2d::~Segment2d() {}

Vector2d Segment2d::intersection(Segment2d a, Segment2d b)
{
	double a_slope = a.director().y() / a.director().x();
	double a_intercept = a.origin().y() - a_slope * a.origin().x();

	double b_slope = b.director().y() / b.director().x();
	double b_intercept = b.origin().y() - b_slope * b.origin().x();

	double x = (b_intercept - a_intercept) / (a_slope - b_slope);

	// Check if x is within a
	double min = (a.director().x() > 0 ? a.origin().x() : a.origin().x() + a.director().x());
	double max = (a.director().x() > 0 ? a.origin().x() + a.director().x() : a.origin().x());

	if (x < min || x > max)
		return Vector2d(0, 0);

	// Check if x is within b
	min = (b.director().x() > 0 ? b.origin().x() : b.origin().x() + b.director().x());
	max = (b.director().x() > 0 ? b.origin().x() + b.director().x() : b.origin().x());
	
	if (x < min || x > max)
		return Vector2d(0, 0);

	return Vector2d(x, a_slope * x + a_intercept);
}

void Segment2d::render()
{
	al_draw_line(origin_.x(), origin_.y(), origin_.x() + director_.x(), origin_.y() + director_.y(), al_map_rgb(255, 0, 255), 0);

	al_draw_filled_circle(origin_.x(), origin_.y(), 3, al_map_rgb(0, 255, 0));
	al_draw_filled_circle(origin_.x() + director_.x(), origin_.y() + director_.y(), 3, al_map_rgb(0, 255, 0));
}