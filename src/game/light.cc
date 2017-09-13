#include "light.h"

#include <algorithm>
#include <vector>

#include <cmath>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "game.h"
#include "input.h"
#include "vector.h"

Light::Light()
	: hc::Object(TYPE_ID)
{
	masks.push_back(mask());
	masks.back().points.push_back(pt(0, 0));
	masks.back().points.push_back(pt(840, 0));
	masks.back().points.push_back(pt(840, 360));
	masks.back().points.push_back(pt(0, 360));
	masks.back().points.push_back(pt(0, 0));

	masks.push_back(mask());
	masks.back().points.push_back(pt(100, 150));
	masks.back().points.push_back(pt(120, 50));
	masks.back().points.push_back(pt(200, 80));
	masks.back().points.push_back(pt(140, 210));
	masks.back().points.push_back(pt(100, 150));

	masks.push_back(mask());
	masks.back().points.push_back(pt(100, 200));
	masks.back().points.push_back(pt(120, 250));
	masks.back().points.push_back(pt(60, 300));
	masks.back().points.push_back(pt(100, 200));

	masks.push_back(mask());
	masks.back().points.push_back(pt(200, 260));
	masks.back().points.push_back(pt(220, 150));
	masks.back().points.push_back(pt(300, 200));
	masks.back().points.push_back(pt(350, 320));
	masks.back().points.push_back(pt(200, 260));

	masks.push_back(mask());
	masks.back().points.push_back(pt(540, 60));
	masks.back().points.push_back(pt(560, 40));
	masks.back().points.push_back(pt(570, 70));
	masks.back().points.push_back(pt(540, 60));

	masks.push_back(mask());
	masks.back().points.push_back(pt(650, 190));
	masks.back().points.push_back(pt(760, 170));
	masks.back().points.push_back(pt(740, 270));
	masks.back().points.push_back(pt(630, 290));
	masks.back().points.push_back(pt(650, 190));

	masks.push_back(mask());
	masks.back().points.push_back(pt(600, 95));
	masks.back().points.push_back(pt(780, 50));
	masks.back().points.push_back(pt(680, 150));
	masks.back().points.push_back(pt(600, 95));

	vertices = (ALLEGRO_VERTEX*)malloc(sizeof(ALLEGRO_VERTEX) * 200);
	shadows = al_create_bitmap(1280, 720);
}

Light::~Light()
{
	al_destroy_bitmap(shadows);
	free(vertices);
}

seg Light::get_intersection(seg r, seg s)
{
	// Are they parallel? If so, no intersect
	float r_mag = sqrt(r.dx * r.dx + r.dy * r.dy);
	float s_mag = sqrt(s.dx * s.dx + s.dy * s.dy);

	if (r.dx / r_mag == s.dx / s_mag && r.dy / r_mag == s.dy / s_mag)
		return seg(NAN, NAN, NAN, NAN, NAN);

	s.t = (r.dx * (s.py - r.py) + r.dy * (r.px - s.px)) / (s.dx * r.dy - s.dy * r.dx);
	r.t = (s.px + s.dx * s.t - r.px) / r.dx;

	// Must be within parametic whatevers for RAY/SEGMENT
	if (r.t < 0)
		return seg(NAN, NAN, NAN, NAN, NAN);
	if (s.t < 0 || s.t > 1)
		return seg(NAN, NAN, NAN, NAN, NAN);

	// Return the POINT OF INTERSECTION
	return r;
}

std::vector<seg> Light::get_sight_polygon(float sightX, float sightY)
{
	std::vector<pt> points;

	for (mask& m : masks) {
		for (pt& p : m.points) {
			bool found = false;
			for (pt r : points) {
				if (r.x == p.x && r.y == p.y) {
					found = true;
					break;
				}
			}

			if (!found)
				points.push_back(p);
		}
	}

	std::vector<float> angles;
	for (pt& p : points) {
		float angle = atan2(p.y - sightY, p.x - sightX);
		angles.push_back(angle);
		angles.push_back(angle - 0.001);
		angles.push_back(angle + 0.001);
	}

	std::vector<seg> intersects;

	for (float& a : angles) {
		seg ray = seg(sightX, sightY, cos(a), sin(a), 1);

		seg closest = seg(NAN, NAN, NAN, NAN, NAN);
		for (mask& m : masks) {
			for (unsigned int j = 1; j < m.points.size(); j++) {
				pt& a = m.points[j];
				pt& b = m.points[j - 1];

				seg inter = get_intersection(ray, seg(b.x, b.y, a.x - b.x, a.y - b.y, 1));
				if (inter.null())
					continue;
				if (closest.null() || inter.t < closest.t)
					closest = inter;
			}
		}

		if (closest.null())
			continue;

		intersects.push_back(closest);
	}

	std::sort(intersects.begin(), intersects.end(), [](seg a, seg b) { return atan2(a.dy, a.dx) < atan2(b.dy, b.dx); });
	// Polygon is intersects, in order of angle
	return intersects;
}

void Light::update()
{
}

void Light::render()
{
	hc::Input& input = hc::Game::game().input();

	al_draw_filled_rectangle(0, 0, 840,360, al_map_rgb(200, 127, 0));

	for (mask& m : masks) {
		for (unsigned int j = 1; j < m.points.size(); j++) {
			al_draw_line(m.points[j - 1].x, m.points[j - 1].y, m.points[j].x, m.points[j].y, al_map_rgb(255, 255, 255), 4);
		}
	}

	al_set_target_bitmap(shadows);
	al_clear_to_color(al_map_rgb(255, 255, 255));

	render_light(input.mx(), input.my());
	for (float a = 0; a < 2 * PI; a += PI / 16)
		render_light(input.mx() + 4 * cos(a), input.my() + 4 * sin(a));

	al_set_target_backbuffer(hc::Game::game().display());

	al_set_blender(ALLEGRO_DEST_MINUS_SRC, ALLEGRO_ONE, ALLEGRO_ONE);
	al_draw_bitmap(shadows, 0, 0, 0);
	al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_INVERSE_ALPHA);
}

void Light::render_light(float x, float y)
{
	std::vector<seg> rays = get_sight_polygon(x, y);
	rays.push_back(rays.front());

	ALLEGRO_COLOR col = al_premul_rgba(0, 0, 0, 255 / 33);

	vertices[0].x = x;
	vertices[0].y = y;
	vertices[0].z = 0;
	vertices[0].color = col;
	vertices[0].u = 0;
	vertices[0].v = 0;

	int i = 1;
	for (seg& r : rays) {
		vertices[i].x = r.px + r.dx * r.t;
		vertices[i].y = r.py + r.dy * r.t;
		vertices[i].z = 0;
		vertices[i].color = col;
		vertices[i].u = 0;
		vertices[i].v = 0;

		i++;
	}

	al_draw_prim(vertices, NULL, NULL, 0, i, ALLEGRO_PRIM_TRIANGLE_FAN);
}