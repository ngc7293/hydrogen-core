#include "test.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "boundingpoly.h"
#include "game.h"
#include "input.h"
#include "manager.h"
#include "segment.h"

Test::Test(float x, float y)
	: Object(TEST)
	, x_(x)
	, y_(y)
	, poly_(x, y)
{
	hc::Vector p1(-16, -32);
	hc::Vector p2( 16 + (rand() % 48), -32);
	hc::Vector p3((rand() % 48) - 24, (rand() % 48) + 16);

	poly_.add(hc::Segment(p1.x(), p1.y(), p2.x(), p2.y()));
	poly_.add(hc::Segment(p1.x(), p1.y(), p3.x(), p3.y()));
	poly_.add(hc::Segment(p3.x(), p3.y(), p2.x(), p2.y()));
}

Test::~Test()
{
}

void Test::update()
{
	Test* first = static_cast<Test*>(hc::Game::game()->manager()->first(TEST));
	if (first == this) {
		hc::Input* input = hc::Game::game()->input();
		poly_.move(input->mx(), input->my());
		x_ = input->mx();
		y_ = input->my();

		if (input->isKey(hc::Input::DOWN, ALLEGRO_KEY_SPACE))
			poly_.rotate(PI / 180);
	}

	std::vector<Object*> tests = hc::Game::game()->manager()->all(TEST);
	for (unsigned int i(0); i < tests.size(); i++) {
		Test* t = static_cast<Test*>(tests[i]);
		if (t != this)
			hc::BoundingPoly::collision(poly_, t->poly());
		if (first == this && t != this)
			hc::BoundingPoly::mindistance(poly_, t->poly());
	}
}

void Test::render()
{
	poly_.render();
}