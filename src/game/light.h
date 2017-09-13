#ifndef LIGHT_H_
#define LIGHT_H_

#include "object.h"

#include <vector>

#include <cmath>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

typedef struct Seg {
	float px, py;
	float dx, dy;
	float t;

	Seg(float px, float py, float dx, float dy, float t)
		: px(px)
		, py(py)
		, dx(dx)
		, dy(dy)
		, t(t)
	{
	}

	bool null() { return std::isnan(t); }
} seg;

typedef struct Point {
	float x, y;

	Point(float x, float y)
		: x(x)
		, y(y)
	{
	}
} pt;

struct mask {
	std::vector<pt> points;

	mask() {}
};

class Light : public hc::Object {
public:
	const int TYPE_ID = 1;

private:
	std::vector<mask> masks;
	ALLEGRO_VERTEX* vertices;
	ALLEGRO_BITMAP* shadows;

	unsigned int rays_;
	std::vector<pt> lights_;

public:
	Light();
	~Light();

	virtual void update();
	virtual void render();

	void render_light(float x, float y);

	seg get_intersection(seg r, seg s);
	std::vector<seg> get_sight_polygon(float sightX, float sightY);
};

#endif