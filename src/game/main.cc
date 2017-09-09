#include "game.h"

#include "vector.h"
int main(int argc, char const* argv[])
{
	// I dont want to do this but my vector algebra fails without it
	vecf::USE_PERMISSIVE_EQUALITY = true;

	// Setup the Game object
	hc::Game& game = hc::Game::game();

	/* Run the game loop */
	while (game.loop())
		;

	return 0;
}