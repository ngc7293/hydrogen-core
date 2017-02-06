#include "view.h"

#include <cmath>

#include "vector.h"

#include "game.h"
#include "input.h"

namespace hc {

vecf View::onDisplay(vecf pos)
{
	hc::View& view = hc::Game::game().view();
	return (pos - view.pos()) * view.scale();
}

View::View()
{
	pos_ = vecf(-DEFAULT_SCREEN_HEIGHT * 0.5, -DEFAULT_SCREEN_HEIGHT * 0.5);
	scale_ = 1;

	minpos_ = vecf(pos_.x(), pos_.y());
	maxpos_ = vecf(pos_.x(), pos_.y());

	movement_ = false;
	scaling_ = false;
}

View::~View()
{
}

void View::update()
{
	if (movement_)
		move();

	if (scaling_)
		zoom();
}

void View::move()
{
	hc::Input& input = hc::Game::game().input();

	float xto(0), yto(0);
	if (input.isKey(hc::Input::DOWN, ALLEGRO_KEY_W))
		yto--;
	if (input.isKey(hc::Input::DOWN, ALLEGRO_KEY_S))
		yto++;

	if (input.isKey(hc::Input::DOWN, ALLEGRO_KEY_A))
		xto--;
	if (input.isKey(hc::Input::DOWN, ALLEGRO_KEY_D))
		xto++;

	//FIXME: Diagonal moving is faster by a factor of (sqrt(2))
	pos_ += vecf(xto, yto) * (VIEW_SPEED / scale_);

	// Limit view to minpos and maxpos
	if (pos_.x()< minpos_.x() - (DEFAULT_SCREEN_WIDTH / scale_ * 0.5))
		pos_.set_x(minpos_.x() - (DEFAULT_SCREEN_WIDTH / scale_ * 0.5));
	if (pos_.y() < minpos_.y() - (DEFAULT_SCREEN_HEIGHT / scale_ * 0.5))
		pos_.set_y(minpos_.y() - (DEFAULT_SCREEN_HEIGHT / scale_ * 0.5));

	if (pos_.x() > maxpos_.x() - (DEFAULT_SCREEN_WIDTH / scale_ * 0.5))
		pos_.set_x(maxpos_.x() - (DEFAULT_SCREEN_WIDTH / scale_ * 0.5));
	if (pos_.y() > maxpos_.y() - (DEFAULT_SCREEN_HEIGHT / scale_ * 0.5))
		pos_.set_y(maxpos_.y() - (DEFAULT_SCREEN_HEIGHT / scale_ * 0.5));

}

void View::zoom()
{
	hc::Input& input = hc::Game::game().input();
	float previous = scale_;

	if (input.mz() > VIEW_SCROLL_MAX)
		input.set_mz(VIEW_SCROLL_MAX);
	if (input.mz() < VIEW_SCROLL_MIN)
		input.set_mz(VIEW_SCROLL_MIN);

	exponent_ = (input.mz() / VIEW_SCROLL_STEP);
	scale_ = pow(VIEW_EXP_BASE, exponent_);

	if (previous != scale_)
		pos_ -= vecf(
			(DEFAULT_SCREEN_WIDTH / scale_ - DEFAULT_SCREEN_WIDTH / previous) * 0.5,
			(DEFAULT_SCREEN_HEIGHT / scale_ - DEFAULT_SCREEN_HEIGHT / previous) * 0.5);
}

} // namespace hc