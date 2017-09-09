#include "game.h"

#include <iostream>

#include "manager.h"
#include "vector.h"

#include "polygon.h"

int main(int argc, char const* argv[])
{
	// I dont want to do this but my vector algebra fails without it
	vecf::USE_PERMISSIVE_EQUALITY = true;

	// Setup the Game object
	hc::Game& game = hc::Game::game();

	for (int i = 32; i < 1280; i += 64) {
		hc::Polygon* b = new hc::Polygon(vecf(i, 32));

		b->add(vecf(-32,-32));
		b->add(vecf(+32,-32));
		b->add(vecf(+32,+32));
		b->add(vecf(-32,+32));
		game.manager().add(b);
	}

	hc::Polygon* c = new hc::Polygon(vecf(300, 200), true);

	c->add(vecf(-32,-32));
	c->add(vecf(+32,-32));
	c->add(vecf(+32,+32));
	c->add(vecf(-32,+32));

	game.manager().add(c);

	/* Run the game loop */
	while (game.loop())
		;

	return 0;
}