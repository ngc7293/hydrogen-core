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

	Point(float x = 0, float y = 0)
		: x(x)
		, y(y)
	{
	}
} pt;

struct mask {
	std::vector<pt> points;
	pt center;
	float radius;

	mask() {}
	void update_radius() {
		center.x = 0;
		center.y = 0;
		for (unsigned int i = 1; i < points.size(); i++) {
			center.x += points[i].x;
			center.y += points[i].y;
		}
		center.x /= points.size() - 1;
		center.y /= points.size() - 1;

		radius = 0;
		for (unsigned int i = 1; i < points.size(); i++) {
			float dis = sqrt(pow(points[i].x - center.x,2) + pow(points[i].y - center.y,2));
			if (radius < dis)
				radius = dis;
		}
		radius *= radius;
	}
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

	void render_light(float x, float y, float r);

	seg get_intersection(seg r, seg s);
	std::vector<seg> get_sight_polygon(float sightX, float sightY, float r);
};

#endif