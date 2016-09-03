#include <ctime>
#include <iostream>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

#include "game.h"

int main(int argc, char const* argv[])
{
    srand(time(0));

    /* Initiate the allegro library and addons */
    al_init();
    al_init_image_addon();
    al_init_primitives_addon();
    al_install_keyboard();
    al_install_mouse();

    /* Setup the Game object */
    Game* game = Game::game();

    /* Run the game loop */
    while (game->loop())
        ;

    return 0;
}