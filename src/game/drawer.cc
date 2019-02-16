#include "drawer.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "game.h"
#include "input.h"

Drawer::Drawer()
    : Object(TYPE_ID)
{
    draw_wave_ = true;
    draw_lines_ = true;
    draw_bg_ = true;
    line_count_ = 10;

    bg_ = new hc::Sprite("bg.png");
}

Drawer::~Drawer()
{
    delete bg_;
    clear();
}

void Drawer::update()
{
    hc::Input& input = hc::Game::game().input();

    if (input.isMouseButton(hc::Input::DOWN, 0)) {
        if (segments_.size() == 0) {
            segments_.push_back(new hc::Segment(0,720/2,input.mx(), input.my()));
        }
        else if (input.mx() > segments_.back()->end().x()) {
            vecf last = segments_.back()->end();
            segments_.push_back(new hc::Segment(last.x(), last.y(), input.mx(), input.my()));
        }
    }

    if (input.isKey(hc::Input::RELEASED, ALLEGRO_KEY_R)) {
        clear();
    }

    if (input.isKey(hc::Input::RELEASED, ALLEGRO_KEY_W)) {
        draw_wave_ = !draw_wave_;
    }

    if (input.isKey(hc::Input::RELEASED, ALLEGRO_KEY_L)) {
        draw_lines_ = !draw_lines_;
    }

    if (input.isKey(hc::Input::RELEASED, ALLEGRO_KEY_B)) {
        draw_bg_ = !draw_bg_;
    }

    if (input.isKey(hc::Input::RELEASED, ALLEGRO_KEY_EQUALS) && input.isKey(hc::Input::DOWN, ALLEGRO_KEY_RSHIFT)) {
        line_count_++;
    }
    if (input.isKey(hc::Input::RELEASED, ALLEGRO_KEY_MINUS) && line_count_ > 1) {
        line_count_--;
    }
}

void Drawer::render()
{
    if (draw_bg_) {
        bg_->draw(vecf(300,0));
    }

    if (draw_wave_) {
        if (segments_.size() == 0) {
            al_draw_circle(0,720/2,4,al_map_rgb(255,255,255), 2);
        }
        else {
            for (hc::Segment* s : segments_) {
                al_draw_line(s->pos().x(), s->pos().y(), s->end().x(), s->end().y(), al_map_rgb(255,255,255), 2);
            }
        }
    }

    for (int i = 1; i < line_count_ + 1; i++) {
        float x = i * (1280/(line_count_ + 1));

        hc::Segment line = hc::Segment(x, 0, x, 720);

        if (draw_lines_) {
            al_draw_line(line.pos().x(), line.pos().y(), line.end().x(), line.end().y(), al_map_rgb(255,0,255), 2);
        }

        for (hc::Segment* s : segments_) {
            if (hc::Segment::intersection(line, *s)) {
                vecf point = hc::Segment::intersection_point(line, *s);
                al_draw_filled_circle(point.x(), point.y(), 4, al_map_rgb(255,0,0));
            }
        }
    }
}

void Drawer::clear()
{
    for (hc::Segment* s : segments_) {
        delete s;
    }
    segments_.clear();
}