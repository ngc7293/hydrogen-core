#include "perlin.h"

#include <algorithm>
#include <random>

#include <iostream>

#include <cmath>

namespace hc {

std::vector<int> Perlin::p = { 0 };

void Perlin::seed(unsigned int seed)
{
	p.clear();
	p.resize(256);
	std::iota(p.begin(), p.end(), 0);
	std::shuffle(p.begin(), p.end(), std::default_random_engine(seed));
	p.insert(p.end(), p.begin(), p.end());
}

float Perlin::noise(vec2 pos)
{
	return noise(pos.x(), pos.y());
}

float Perlin::noise(float x, float y)
{
	int X = (int)floor(x) & 255;
	int Y = (int)floor(y) & 255;

	x -= floor(x);
	y -= floor(y);

	float u = fade(x);
	float v = fade(y);

	int A = p[X] + Y;
	int B = p[X + 1] + Y;

	float res = lerp(0, lerp(v, lerp(u, grad(p[A], x, y, 0), grad(p[B], x - 1, y, 0)),
							lerp(u, grad(p[A + 1], x, y - 1, 0), grad(p[B + 1], x - 1, y - 1, 0))),
		lerp(v, lerp(u, grad(p[A + 1], x, y, -1), grad(p[A + 1], x - 1, y, -1)),
			lerp(u, grad(p[A + 2], x, y - 1, -1), grad(p[B + 2], x - 1, y - 1, -1))));
	return (res + 1.0) / 2.0;
}

float Perlin::fade(float t)
{
	return t * t * t * (t * (t * 6 - 15) + 10);
}

float Perlin::lerp(float t, float a, float b)
{
	return a + t * (b - a);
}

float Perlin::grad(int hash, float x, float y, float z)
{
	int h = hash & 15;
	float u = (h < 8 ? x : y);
	float v = (h < 4 ? y : (h == 12 || h == 14 ? x : z));
	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}
}