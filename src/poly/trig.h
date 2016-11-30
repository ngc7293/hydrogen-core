#include "vector.h"

#ifndef TRIG_H_
#define TRIG_H_

namespace trig  {

float distance(hc::vec2 pos, hc::vec2 pos2);
float distance_sq(hc::vec2 pos, hc::vec2 pos2);

float distance(float x, float y, float x2, float y2);
float distance_sq(float x, float y, float x2, float y2);

} //namespace trig

#endif