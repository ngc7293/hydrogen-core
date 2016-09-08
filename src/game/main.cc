#include "game.h"

#include "manager.h"
#include "test.h"

int main(int argc, char const* argv[])
{
	// Setup the Game object
	hc::Game* game = hc::Game::game();

		game->manager()->add(new Test(DEFAULT_SCREEN_WIDTH / 2, DEFAULT_SCREEN_HEIGHT / 2, 0, 0));

	game->manager()->add(new Test(120, 120, 360, 523));
	game->manager()->add(new Test(528, 64, Vector2d(-428, 427)));
	game->manager()->add(new Test(600, 600, Vector2d(40, 40)));
	game->manager()->add(new Test(600, 640, Vector2d(40, -40)));

	// Run the game loop
	while (game->loop())
		;

	return 0;
}