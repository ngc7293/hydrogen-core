#ifndef PERLIN_H_
#define PERLIN_H_

#include <vector>

#include "util/vector.hh"

namespace hc {

class Perlin {
public:
	static std::vector<int> p;

public:
	static void seed(unsigned int seed);
	static float noise(vecf pos);
	static float noise(float x, float y, float z);

private:
	static float fade(float t);
	static float lerp(float t, float a, float b);
	static float grad(int hash, float x, float y, float z);
};

}

#endif 