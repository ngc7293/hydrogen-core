#include "light.h"

#include <algorithm>
#include <vector>

#include <cmath>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_color.h>

#include "game.h"
#include "input.h"
#include "perlin.h"
#include "vector.h"

Light::Light()
	: hc::Object(TYPE_ID)
{
	masks.push_back(mask());
	masks.back().points.push_back(pt(0, 0));
	masks.back().points.push_back(pt(1280, 0));
	masks.back().points.push_back(pt(1280, 720));
	masks.back().points.push_back(pt(0, 720));
	masks.back().points.push_back(pt(0, 0));
	masks.back().update_radius();

	for (int i = 0; i < 18; i++) {
		for (int j = 0; j < 10; j++) {
			float p = hc::Perlin::noise(i + 0.1, j + 0.1, 1);
			if (p >= 0.5) {
				masks.push_back(mask());
				masks.back().points.push_back(pt(64 + i * 64, 40 + j * 64));
				masks.back().points.push_back(pt(128 + i * 64, 40 + j * 64));
				masks.back().points.push_back(pt(128 + i * 64, 104 + j * 64));
				masks.back().points.push_back(pt(64 + i * 64, 104 + j * 64));
				masks.back().points.push_back(pt(64 + i * 64, 40 + j * 64));
				masks.back().update_radius();
			}
		}
	}

	vertices = (ALLEGRO_VERTEX*)malloc(sizeof(ALLEGRO_VERTEX) * 1000);
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

std::vector<seg> Light::get_sight_polygon(float sightX, float sightY, float r)
{
	std::vector<pt> points;

	for (mask& m : masks) {
		if (m.radius + ((m.center.x - sightX) * (m.center.x - sightX) + (m.center.y - sightY) * (m.center.y - sightY)) > r * r) {
			continue;
		}
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

	for (float a = 0; a < 2 * PI; a += PI/8) {
		points.push_back(pt(sightX + (2 * r * cos(a)), sightY + (2 * r * sin(a))));
	}

	if (points.size() == 0)
		return std::vector<Seg>();

	hc::Game::game().debug_osd("unique points: " + std::to_string(points.size()) + " out of a maximum of " + std::to_string(masks.size() * 4));

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
			if (m.radius + ((m.center.x - sightX) * (m.center.x - sightX) + (m.center.y - sightY) * (m.center.y - sightY)) > r * r) {
			continue;
		}
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

		if (closest.t > r)
			closest.t = r;

		intersects.push_back(closest);
	}

	std::sort(intersects.begin(), intersects.end(), [](seg a, seg b) { return atan2(a.dy, a.dx) < atan2(b.dy, b.dx); });
	// Polygon is intersects, in order of angle
	return intersects;
}

void Light::update()
{
	hc::Input& input = hc::Game::game().input();

	if (input.isMouseButton(hc::Input::PRESSED, 0)) {
		bool found = false;
		for (unsigned int i = 0; i < lights_.size(); i++) {
			if (sqrt(pow(lights_[i].x - input.mx(), 2) + pow(lights_[i].y - input.my(), 2)) < 8) {
				lights_.erase(lights_.begin() + i);
				found = true;
			}
		}

		if (!found)
			lights_.push_back(pt(input.mx(), input.my()));
	}
}

void Light::render()
{
	hc::Input& input = hc::Game::game().input();

	al_draw_filled_rectangle(0, 0, 1280, 720, al_map_rgb(200, 127, 0));

	int h = 0;
	for (mask& m : masks) {
		for (unsigned int j = 1; j < m.points.size(); j++) {
			al_draw_line(m.points[j - 1].x, m.points[j - 1].y, m.points[j].x, m.points[j].y, al_color_hsv(h, 1, 1), 4);
			h = (h + 30) % 360;
		}
	}
	rays_ = 0;
	int lights = 0;

	al_draw_filled_circle(input.mx(), input.my(), 8, al_map_rgb(255, 255, 255));
	for (pt lt : lights_)
		al_draw_filled_circle(lt.x, lt.y, 8, al_map_rgb(255, 255, 255));

	al_set_target_bitmap(shadows);
	al_clear_to_color(al_map_rgb(200, 200, 200));

	render_light(input.mx(), input.my(), 1280);
	lights++;
	
	for (pt lt : lights_) {
		render_light(lt.x, lt.y, 200);
		lights++;
	}

	al_set_target_backbuffer(hc::Game::game().display());

	al_set_blender(ALLEGRO_DEST_MINUS_SRC, ALLEGRO_ONE, ALLEGRO_ONE);
	al_draw_bitmap(shadows, 0, 0, 0);
	al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_INVERSE_ALPHA);
	hc::Game::game().debug_osd("lights: " + std::to_string(lights) + " rays: " + std::to_string(rays_) + " blocks: " + std::to_string(masks.size()));
}

void Light::render_light(float x, float y, float r)
{
	std::vector<seg> rays = get_sight_polygon(x, y, r);
	if (rays.size() == 0)
		return;

	rays.push_back(rays.front());

	ALLEGRO_COLOR col = al_premul_rgba(0, 0, 0, 255);

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
		rays_++;
	}

	al_draw_prim(vertices, NULL, NULL, 0, i, ALLEGRO_PRIM_TRIANGLE_FAN);
}