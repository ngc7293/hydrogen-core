#ifndef PERLIN_H_
#define PERLIN_H_

#include <vector>

#include "vector.h"

namespace hc {

class Perlin {
public:
	static std::vector<int> p;

public:
	static void seed(unsigned int seed);
	static float noise(vec2 pos);
	static float noise(float x, float y);

private:
	static float fade(float t);
	static float lerp(float t, float a, float b);
	static float grad(int hash, float x, float y, float z);
};

}

#endif 