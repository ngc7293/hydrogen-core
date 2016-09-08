#include "test.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "game.h"
#include "input.h"
#include "manager.h"
#include "segment.h"
#include "vector.h"

Test::Test(double x, double y, hc::Vector vec)
	: Object(TEST)
	, s_(x, y, vec)
{
}

Test::Test(double x, double y, double x2, double y2)
	: Object(TEST)
	, s_(x, y, x2, y2)
{
}

Test::~Test()
{
}

void Test::update()
{
	Test* t = static_cast<Test*>(hc::Game::game()->manager()->first(TEST));
	if (t == this) {
		hc::Input* input = hc::Game::game()->input();
		s_.direction() = hc::Vector(input->mx() - s_.origin().x(), input->my() - s_.origin().y());
	}
}

void Test::render()
{
	s_.render();

	std::vector<Object*> tests = hc::Game::game()->manager()->all(TEST);

	for (unsigned int i(0); i < tests.size(); i++) {
		Test* t = static_cast<Test*>(tests[i]);

		if (t != this) {
			hc::Vector pos = hc::Segment::intersection(s_, t->s());
			if (!(pos.x() == 0 && pos.y() == 0))
				al_draw_filled_circle(pos.x(), pos.y(), 3, al_map_rgb(255, 0, 0));
		}	
	}
}