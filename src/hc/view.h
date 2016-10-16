#ifndef VIEW_H_
#define VIEW_H_

#include "vector.h"

namespace hc {

class View {
public:
	static const int VIEW_SPEED = 8;
	static const int VIEW_EXP_BASE = 2;

	// View mouse wheel scrolling 
	static const int VIEW_SCROLL_STEP = 8;
	static const int VIEW_SCROLL_MIN = -8;
	static const int VIEW_SCROLL_MAX = 8;
	
private:
	// Position of top left corner
	vecf pos_;
	vecf minpos_;
	vecf maxpos_;

	// The ratio of units per pixel
	float scale_;
	float exponent_;

	// Enables or disables WSAD viewport movement. False by default.
	bool movement_;

	// Enables or disables scroll-wheel scaling. False by default.
	bool scaling_;

public:
	static vecf onDisplay(vecf pos);

public:
	View();
	~View();

	void update();
	void move();
	void zoom();

	// Access to members
	vecf& pos() { return pos_; }
	vecf& minpos() { return minpos_; }
	vecf& maxpos() { return maxpos_; }

	float scale() const { return scale_; }

	// Return if WASD movement is enabled
	bool movement() { return movement_; }
	void set_movement(bool movement) { movement_ = movement; }

	// Return if scroll wheel scaling/zooming is enabled
	bool scaling() { return scaling_; }
	void set_scaling(bool scaling) { scaling_ = scaling; }
};

} // namespace hc

#endif