#include "test.h"

#include <iostream>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "game.h"
#include "input.h"
#include "manager.h"
#include "polygon.h"
#include "segment.h"

Test::Test(float x, float y)
	: Object(TEST)
	, x_(x)
	, y_(y)
	, poly_(x, y)
{
	hc::Vector p1(-32, -32);
	hc::Vector p2(+32, -32);
	hc::Vector p3(+32, +32);
	hc::Vector p4(-32, +32);

	poly_.add(p1);
	poly_.add(p2);
	poly_.add(p3);
	poly_.add(p4);
}

Test::~Test()
{
}

void Test::update()
{
	Test* first = static_cast<Test*>(hc::Game::game()->manager()->first(TEST));
	if (first == this) {
		hc::Input* input = hc::Game::game()->input();
		if (input->isKey(hc::Input::DOWN, ALLEGRO_KEY_SPACE))
			poly_.rotate(PI / 180);
		move();
	}
}

void Test::render()
{
	poly_.render();
}

void Test::move()
{
	hc::Input* input = hc::Game::game()->input();

	float xto(0), yto(0);
	if (input->isKey(hc::Input::DOWN, ALLEGRO_KEY_W))
		yto--;
	if (input->isKey(hc::Input::DOWN, ALLEGRO_KEY_S))
		yto++;
	if (input->isKey(hc::Input::DOWN, ALLEGRO_KEY_A))
		xto--;
	if (input->isKey(hc::Input::DOWN, ALLEGRO_KEY_D))
		xto++;

	hc::Vector motion(xto, yto);
	motion.set_length(4);

	if (xto == 0 && yto == 0)
		return;

	float factor(1);
	std::vector<Object*> tests = hc::Game::game()->manager()->all(TEST);
	for (unsigned int i(1); i < tests.size(); i++) {
		if (poly_.collision(static_cast<Test*>(tests[i])->poly()))
			std::cout << "collision" << std::endl;
		float cur = poly_.move_until(motion, static_cast<Test*>(tests[i])->poly());
		factor = (cur < factor ? cur : factor);
	}
	motion = motion * (factor - 0.01f);
	x_ += motion.x();
	y_ += motion.y();
	poly_.move(x_, y_);
}