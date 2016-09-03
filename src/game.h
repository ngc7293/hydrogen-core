#include <allegro5/allegro.h>

#include "defaults.h"

#include "input.h"
#include "manager.h"
#include "media.h"

#ifndef GAME_H_
#define GAME_H_

/* Game
 * This is the main game class. It loads/unloads and run the main loop().
 * It acts as a global access points for all other elements through it's
 * Input, Media and Manager module. */
class Game {
private:
    /* Main display */
    ALLEGRO_DISPLAY* display_;

    /* The game tick clock, used for the update() (and render) calls */
    ALLEGRO_TIMER* timer_;

    /* The global user interaction and gametick eventqueue */
    ALLEGRO_EVENT_QUEUE* eventqueue_;

    /* The game will loop() as long as this is true */
    bool run_;

    /* Flag for a pending render */
    bool render_;

    /* Modules : see individual files for details */
    Input* input_;
    Manager* manager_;
    Media* media_;

private:
    Game();

public:
    /* Singleton access to the game */
    static Game* game()
    {
        static Game* game_ = new Game();
        return game_;
    }
    ~Game();

    /* The main game loop. Return false if the game should end */
    bool loop();

    /* Return the input module */
    Input* input() { return input_; }

    /* Return the object managing module */
    Manager* manager() { return manager_; }

    /* Return the media IO module */
    Media* media() { return media_; }

    /* Return the main display */
    ALLEGRO_DISPLAY* display() { return display_; }
};

#endif