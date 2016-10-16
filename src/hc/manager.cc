#include "manager.h"

#include <iostream>
#include <vector>

#include "object.h"

namespace hc {

Manager::Manager() {}

Manager::~Manager()
{
	while (objects_.size()) {
		delete objects_[0];
		objects_.erase(objects_.begin());
	}
}

void Manager::add(Object* object)
{
	if (find(object) == -1) {
		unsigned int i(0);
		for (; i < objects_.size() && objects_[i]->type() < object->type(); i++)
		objects_.insert(objects_.begin() + i, object);
	}
}

void Manager::remove(Object* object)
{
	for (unsigned int i(0); i < objects_.size(); i++) {
		if (objects_[i] == object) {
			objects_.erase(objects_.begin() + i);
			break;
		}
	}
}

int Manager::find(Object* object)
{
	if (object == nullptr)
		return -1;

	for (unsigned int i(0); i < objects_.size(); i++)
		if (objects_[i] == object)
			return i;
	return -1;
}

Object* Manager::first(Object::Type filter)
{
	for (unsigned int i(0); i < objects_.size(); i++)
		if (objects_[i]->type() == filter)
			return objects_[i];
	return nullptr;
}

std::vector<Object*> Manager::all(Object::Type filter)
{
	std::vector<Object*> found;

	for (unsigned int i(0); i < objects_.size(); i++)
		if (objects_[i]->type() == filter)
			found.push_back(objects_[i]);
	return found;
}

void Manager::update()
{
	for (unsigned int i(0); i < objects_.size(); i++)
		objects_[i]->update();

	for (unsigned int i(0); i < objects_.size(); i++) {
		if (objects_[i]->destroy()) {
			delete objects_[i];
			objects_.erase(objects_.begin() + i);
			i--;
		}
	}
}

void Manager::render()
{
	for (unsigned int i(0); i < objects_.size(); i++)
		objects_[i]->render();
}

} //namespace hc