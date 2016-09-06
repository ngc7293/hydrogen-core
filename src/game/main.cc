#include "game.h"

int main(int argc, char const* argv[])
{
	/* Setup the Game object */
	Game* game = Game::game();

	/* Run the game loop */
	while (game->loop())
		;

	return 0;
}