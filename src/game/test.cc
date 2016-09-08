#include "test.h"

#include <iostream>

Test::Test()
	: Object(TEST)
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
		s_.director() = Vector2d(input->mx() - s_.origin().x(), input->my() - s_.origin().y());
	}
}

void Test::render()
{
}

	std::vector<Object*> tests = hc::Game::game()->manager()->all(TEST);

	for (unsigned int i(0); i < tests.size(); i++) {
		Test* t = static_cast<Test*>(tests[i]);

		if (t != this) {
			Vector2d pos = Segment2d::intersection(s_, t->s());
			if (!(pos.x() == 0 && pos.y() == 0))
				al_draw_filled_circle(pos.x(), pos.y(), 3, al_map_rgb(255, 0, 0));
		}	
	}
}