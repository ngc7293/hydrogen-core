#include "object.h"

#ifndef TEST_H_
#define TEST_H_

class Test : public Object {
private:
public:
	Test();
	virtual ~Test();

	virtual void update();
	virtual void render();

	void talktome();
};

#endif