#include "game.h"

#include <iostream>

#include "manager.h"
#include "vector.h"

#include "test.h"

int main(int argc, char const* argv[])
{
	// Setup the Game object
	hc::Game& game = hc::Game::game();

	game.manager().add(new Test(DEFAULT_SCREEN_WIDTH / 2, DEFAULT_SCREEN_HEIGHT / 2));
	game.manager().add(new Test(528, 050));
	game.manager().add(new Test(528, 064));
	game.manager().add(new Test(120, 120));
	while (game.loop())
		;

	return 0;
}