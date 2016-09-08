#include "test.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "game.h"
#include "input.h"
#include "manager.h"
#include "boundingpoly.h"
#include "segment.h"

Test::Test(double x, double y)
    : Object(TEST)
    , x_(x)
    , y_(y)
    , poly_(x, y)
{
	poly_.add(hc::Segment(x-16, y - 16, x + 64, y-16));
	poly_.add(hc::Segment(x + 64, y - 16, x, y + 64));
	poly_.add(hc::Segment(x, y + 64, x - 64, y));
	poly_.add(hc::Segment(x - 64, y, x - 16	, y - 16));
}

Test::~Test()
{
}

void Test::update()
{
	Test* t = static_cast<Test*>(hc::Game::game()->manager()->first(TEST));
	if (t == this) {
		hc::Input* input = hc::Game::game()->input();
		poly_.move(input->mx() - x_, input->my() - y_);
		x_ = input->mx();
		y_ = input->my();
	}

	std::vector<Object*> tests = hc::Game::game()->manager()->all(TEST);
	for (unsigned int i(0); i < tests.size(); i++) {
		t = static_cast<Test*>(tests[i]);
		if (t != this)
			hc::BoundingPoly::collision(poly_, t->poly());
	}
}

void Test::render()
{
	for (unsigned int i(0); i < poly_.segments().size(); i++)
		poly_.segments()[i].render();
	al_draw_filled_circle(x_, y_, 4, al_map_rgb(255, 255, 255));
}