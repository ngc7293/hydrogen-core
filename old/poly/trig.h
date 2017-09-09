#include "vector.h"

#ifndef TRIG_H_
#define TRIG_H_

namespace trig  {

float distance(vecf pos, vecf pos2);
float distance_sq(vecf pos, vecf pos2);

float distance(float x, float y, float x2, float y2);
float distance_sq(float x, float y, float x2, float y2);

} //namespace trig

#endif