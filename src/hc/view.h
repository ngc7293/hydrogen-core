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
	Vector<float> pos_;
	Vector<float> minpos_;
	Vector<float> maxpos_;

	// The ratio of units per pixel
	float scale_;
	float exponent_;

public:
	static Vector<float> onDisplay(Vector<float> pos);

public:
	View();
	~View();

	void update();
	void move();
	void zoom();

	// Access to members
	Vector<float>& pos() { return pos_; }
	Vector<float>& minpos() { return minpos_; }
	Vector<float>& maxpos() { return maxpos_; }

	float scale() const { return scale_; }
};

} // namespace hc

#endif