#include "object.h"

namespace hc {

Object::Object(Type type)
    : type_(type)
{
	destroy_ = false;
}

Object::~Object() {} // Seppuku

} //namespace hc