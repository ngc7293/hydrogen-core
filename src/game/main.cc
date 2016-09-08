#include "game.h"

int main(int argc, char const* argv[])
{
	// Setup the Game object
	hc::Game* game = hc::Game::game();

	/* Run the game loop */
	while (game->loop())
		;

	return 0;
}