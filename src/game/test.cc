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
		float xto(0), yto(0);
		hc::Input* input = hc::Game::game()->input();
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

		if (input->isKey(hc::Input::DOWN, ALLEGRO_KEY_SPACE))
			poly_.rotate(PI / 180);

		float factor(1);
		std::vector<Object*> tests = hc::Game::game()->manager()->all(TEST);
		for (unsigned int i(1); i < tests.size(); i++) {
			Test* t = static_cast<Test*>(tests[i]);
			float cur = poly_.move_predict(motion, t->poly());
			factor = (cur < factor ? cur : factor);
		}
		motion = motion * (factor - 0.01f);
		x_ += motion.x();
		y_ += motion.y(); 
		poly_.move(x_, y_);
	}

}

void Test::render()
{
	poly_.render();
}