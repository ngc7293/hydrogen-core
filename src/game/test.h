#include "object.h"

#include "polygon.h"

#ifndef TEST_H_
#define TEST_H_

class Test : public hc::Object {
private:
	float size_;
	float x_, y_;
	hc::Polygon poly_;

public:
	Test(float x, float y, bool first = false);
	virtual ~Test();

	virtual void update();
	virtual void render();

	hc::Polygon& poly() { return poly_; }
};

#endif