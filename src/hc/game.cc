#include "game.h"

#include <iostream>

#include <cstdint>
#include <ctime>

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>

#include "input.h"
#include "log.h"
#include "manager.h"
#include "media.h"
#include "perlin.h"
#include "view.h"

namespace hc {

Game::Game()
{
	// Assume user has done no setup
	srand(time(0));
	Perlin::seed(1505266067);

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
	Log::log(Log::INFO, "Game", "Initialized Allegro v" + std::to_string(version >> 24) + "." + std::to_string((version >> 16) & 255) + "." + std::to_string((version >> 8) & 255) + "." + std::to_string(version & 255));

	// Modules
	view_ = new View();
	manager_ = new Manager();
	media_ = new Media();
	input_ = new Input();

	// Flags
	run_ = true;
	render_ = false;

	// Display
	al_set_new_display_option(ALLEGRO_RENDER_METHOD, 1, ALLEGRO_SUGGEST);
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
	font_ = media_->font("mono.ttf", 16);

	// Debug info
	debug_display_fps_ = true;
	debug_last_frame_time_ = al_get_time();

	debug_display_masks_ = false;

	debug_display_osd_ = true;
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
		if (debug_display_fps_) {
			al_draw_filled_rectangle(0, 0, 24, 16, al_map_rgb(0, 0, 0));
			al_draw_textf(font_, al_map_rgb(255, 255, 255), 0, 0, 0, "%i", (int)(1 / (al_get_time() - debug_last_frame_time_)));
			debug_last_frame_time_ = al_get_time();
		}
		if (debug_display_osd_) {
			int y = 16;
			for (std::string str : debug_osd_) {
				al_draw_filled_rectangle(0, y, al_get_text_width(font_, str.c_str()), y + 16, al_map_rgb(0, 0, 0));
				al_draw_textf(font_, al_map_rgb(255, 255, 255), 0, y, 0, "%s", str.c_str());
				y += 16;
			}
			debug_osd_.clear();
		}

		// Flip the buffer, and clear it
		al_flip_display();
		al_clear_to_color(al_map_rgb(0, 0, 0));
	}

	// Debug toggles
	if (event.type == ALLEGRO_EVENT_KEY_CHAR || event.keyboard.modifiers & ALLEGRO_KEYMOD_CTRL) {
		switch (event.keyboard.keycode) {
		case ALLEGRO_KEY_F:
			debug_display_fps_ = !debug_display_fps_;
			break;
		case ALLEGRO_KEY_M:
			debug_display_masks_ = !debug_display_masks_;
			break;
		case ALLEGRO_KEY_O:
			debug_display_osd_ = !debug_display_osd_;
		}
	}

	return run_;
}

} // namespace hc