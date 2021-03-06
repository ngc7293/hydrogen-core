#include "hc/game.hh"

#include <iostream>

#include <cstdint>
#include <ctime>

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>

#include "hc/input.hh"
#include "hc/log.hh"
#include "hc/manager.hh"
#include "hc/media.hh"
#include "hc/view.hh"

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

	// Setup log and broadcast Allegro version
	Log::set_console_level(Log::WARNING);
	Log::set_logfile_level(Log::WARNING);

	uint32_t version = al_get_allegro_version();
	Log::log(Log::INFO, "hc/game", "Initialized Allegro v" + std::to_string(version >> 24) + "." + std::to_string((version >> 16) & 255) + "." + std::to_string((version >> 8) & 255) + "." + std::to_string(version & 255));

	// Modules
	view_ = new View();
	manager_ = new Manager();
	media_ = new Media();
	input_ = new Input();

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

	// Register event sources
	al_register_event_source(eventqueue_, al_get_display_event_source(display_));
	al_register_event_source(eventqueue_, al_get_timer_event_source(timer_));
	al_register_event_source(eventqueue_, al_get_keyboard_event_source());
	al_register_event_source(eventqueue_, al_get_mouse_event_source());

	// Load on-screen debug info font
	font_ = media_->font("mono.ttf", 12);

	// Debug info
	display_fps_ = true;
	last_frame_ = al_get_time();

	display_masks_ = false;
}

Game::~Game()
{
	media_->unref(font_);

	delete view_;
	delete manager_;
	delete media_;
	delete input_;

	al_destroy_display(display_);
	al_destroy_timer(timer_);
	al_destroy_event_queue(eventqueue_);
}

bool Game::loop()
{
	ALLEGRO_EVENT event;
	al_wait_for_event(eventqueue_, &event);

	input_->handle(event);

	// Quit game on display close or ESC
	if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		return false;
	else if (input_->isKey(Input::RELEASED, ALLEGRO_KEY_ESCAPE))
		return false;

	// Game tick
	else if (event.type == ALLEGRO_EVENT_TIMER) {
		view_->update();
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

		// FPS counter display
		if (display_fps_) {
			al_draw_filled_rectangle(0, 0, 16, 12, al_map_rgb(0, 0, 0));
			al_draw_textf(font_, al_map_rgb(255, 255, 255), 0, 0, 0, "%i", (int)(1 / (al_get_time() - last_frame_)));
			last_frame_ = al_get_time();
		}

		// Flip the buffer, and clear it
		al_flip_display();
		al_clear_to_color(al_map_rgb(0, 0, 0));
	}

	// Debug toggles
	if (event.type == ALLEGRO_EVENT_KEY_CHAR || event.keyboard.modifiers & ALLEGRO_KEYMOD_CTRL) {
		switch (event.keyboard.keycode) {
		case ALLEGRO_KEY_B:
			display_fps_ = !display_fps_;
			break;
		case ALLEGRO_KEY_M:
			display_masks_ = !display_masks_;
			break;
		}
	}

	return run_;
}

} // namespace hc