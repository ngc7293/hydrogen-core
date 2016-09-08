#include "object.h"

Object::Object(Type type)
    : type_(type)
{
	destroy_ = false;
}

Object::~Object() { /* Seppuku */}