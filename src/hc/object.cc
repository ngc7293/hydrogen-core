#include "object.h"

namespace hc {

Object::Object(int type)
    : type_(type)
{
	destroy_ = false;
}

Object::~Object() {} // Seppuku

} //namespace hc