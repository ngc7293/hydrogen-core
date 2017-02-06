#include "game.h"

#include <iostream>

#include "manager.h"
#include "vector.h"

#include "test.h"

int main(int argc, char const* argv[])
{
	// Setup the Game object
	hc::Game& game = hc::Game::game();

	/* Run the game loop */
	while (game.loop())
		;

	return 0;
}