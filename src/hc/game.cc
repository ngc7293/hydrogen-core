#include "game.h"

#include <iostream>

#include <ctime>

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>

#include "input.h"
#include "manager.h"

namespace hc {

Game::Game()
{
	// Assume user has done no setup
	srand(time(0));

	// Initiate the allegro library and addons
	al_init();
	al_init_font_addon();
	al_init_image_addon();
	al_init_primitives_addon();
	al_init_ttf_addon();

	// Install input devices
	al_install_keyboard();
	al_install_mouse();

	// Modules
	input_ = new Input();
	manager_ = new Manager();
	media_ = new Media();

	// Flags
	run_ = true;
	render_ = false;

	// Display
	al_set_new_display_option(ALLEGRO_VSYNC, 1, ALLEGRO_SUGGEST);
	al_set_new_display_option(ALLEGRO_SAMPLES, 1, ALLEGRO_SUGGEST);
	al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 8, ALLEGRO_SUGGEST);
	display_ = al_create_display(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT);

	// Timer
	timer_ = al_create_timer(1.0 / DEFAULT_GAME_SPEED);
	al_start_timer(timer_);

	// Event Queue
	eventqueue_ = al_create_event_queue();

	al_register_event_source(eventqueue_, al_get_display_event_source(display_));
	al_register_event_source(eventqueue_, al_get_timer_event_source(timer_));
	al_register_event_source(eventqueue_, al_get_keyboard_event_source());
	al_register_event_source(eventqueue_, al_get_mouse_event_source());
}

Game::~Game()
{
	al_destroy_display(display_);
	al_destroy_timer(timer_);
	al_destroy_event_queue(eventqueue_);

	delete input_;
	delete media_;
	delete manager_;
}

bool Game::loop()
{
	ALLEGRO_EVENT event;
	al_wait_for_event(eventqueue_, &event);

	input_->handle(event);

	if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		return false;

	if (input_->isKey(Input::RELEASED, ALLEGRO_KEY_ESCAPE))
		return false;

	if (event.type == ALLEGRO_EVENT_TIMER) {
		manager_->update();
		input_->update();
		render_ = true;
	}

	// Do not render if there are still events to be processed. Partially
	// prevents the game from lagging because of a long render()
	if (render_ && al_event_queue_is_empty(eventqueue_)) {
		render_ = false;

		// Tell the objects to render
		manager_->render();

		// Flip the buffer, and clear it
		al_flip_display();
		al_clear_to_color(al_map_rgb(0, 0, 0));
	}

	return run_;
}

} // namespace hc