#include "perlin.h"

#include <algorithm>
#include <iostream>
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

float Perlin::noise(vecf pos)
{
	return noise(pos.x(), pos.y(), 0);
}

float Perlin::noise(float x, float y, float z)
{
	int X = ((int)floor(x)) & 255;
	int Y = ((int)floor(y)) & 255;
	int Z = ((int)floor(z)) & 255;

	x -= floor(x);
	y -= floor(y);
	z -= floor(z);

	float u = fade(x);
	float v = fade(y);
	float w = fade(z);

	int A = p[X] + Y, AA = p[A] + Z, AB = p[A + 1] + Z;
	int B = p[X + 1] + Y, BA = p[B] + Z, BB = p[B + 1] + Z;

	return (lerp(w, lerp(v, lerp(u, grad(p[AA  ], x    , y    , z    ),
									grad(p[BA  ], x - 1, y    , z    )),
							lerp(u, grad(p[AB  ], x    , y - 1, z    ),
						   			grad(p[BB  ], x - 1, y - 1, z    ))),
					lerp(v, lerp(u, grad(p[AA+1], x    , y    , z - 1),
				   					grad(p[BA+1], x - 1, y    , z - 1)),
				   			lerp(u, grad(p[AB+1], x    , y - 1, z - 1),
				   		   	    	grad(p[BB+1], x - 1, y - 1, z - 1)))) + 1.0) / 2.0;
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