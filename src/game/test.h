#include "object.h"

#include "boundingpoly.h"

#ifndef TEST_H_
#define TEST_H_

class Test : public hc::Object {
private:
	float x_, y_;
	hc::BoundingPoly poly_;

public:
	Test(float x, float y);
	virtual ~Test();

	virtual void update();
	virtual void render();

	hc::BoundingPoly& poly() { return poly_; }
};

#endif