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
	hc::Vector p2(16 + (rand() % 48), -32);
	hc::Vector p3((rand() % 48) - 24, (rand() % 48) + 16);

	poly_.add_joint(p1);
	poly_.add_joint(p2);
	poly_.add_joint(p3);
}

Test::~Test()
{
}

void Test::update()
{
	Test* first = static_cast<Test*>(hc::Game::game()->manager()->first(TEST));
	if (first == this) {
		hc::Input* input = hc::Game::game()->input();
		if (input->isKey(hc::Input::DOWN, ALLEGRO_KEY_W))
			y_--;
		if (input->isKey(hc::Input::DOWN, ALLEGRO_KEY_S))
			y_++;
		if (input->isKey(hc::Input::DOWN, ALLEGRO_KEY_A))
			x_--;
		if (input->isKey(hc::Input::DOWN, ALLEGRO_KEY_D))
			x_++;

		poly_.move(x_, y_);

		if (input->isKey(hc::Input::DOWN, ALLEGRO_KEY_SPACE))
			poly_.rotate(PI / 180);
		if (input->isKey(hc::Input::PRESSED, ALLEGRO_KEY_ENTER))
			poly_.add_joint(hc::Vector(rand() % 128 - 64, rand() % 128 - 64));
	}

	std::vector<Object*> tests = hc::Game::game()->manager()->all(TEST);
	for (unsigned int i(0); i < tests.size(); i++) {
		Test* t = static_cast<Test*>(tests[i]);
		if (t != this)
			hc::BoundingPoly::collision_points(poly_, t->poly());
		if (first == this && t != this)
			hc::BoundingPoly::mindistance(poly_, t->poly());
	}
}

void Test::render()
{
	poly_.render();
}