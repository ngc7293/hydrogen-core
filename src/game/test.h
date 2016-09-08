#include "object.h"

#include "segment2d.h"

#ifndef TEST_H_
#define TEST_H_

class Test : public hc::Object {
private:
	Segment2d s_;

public:
	Test(double x, double y, Vector2d vec);
	Test(double x, double y, double x2, double y2);
	virtual ~Test();

	virtual void update();
	virtual void render();

	Segment2d& s() { return s_; }
};

#endif