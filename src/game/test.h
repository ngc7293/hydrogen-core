#include "object.h"

#include "segment.h"

#ifndef TEST_H_
#define TEST_H_

class Test : public hc::Object {
private:
	hc::Segment s_;

public:
	Test(double x, double y, hc::Vector vec);
	Test(double x, double y, double x2, double y2);
	virtual ~Test();

	virtual void update();
	virtual void render();

	hc::Segment& s() { return s_; }
};

#endif