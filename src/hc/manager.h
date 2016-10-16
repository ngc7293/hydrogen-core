#include <vector>

#include "object.h"

#ifndef MANAGER_H_
#define MANAGER_H_

namespace hc {

class Manager {
private:
	std::vector<Object*> objects_;

public:
	Manager();
	~Manager();

	// Add an object to the list, keeping the list ordered
	// by object.type() (ascending order)
	void add(Object* object);

	// Remove an object from the list
	void remove(Object* object);

	// Find the position in the array of the object
	int find(Object* object);

	// Return object at position
	//Object* at(unsigned int i);
	template <typename O>
	O* at(unsigned int i);

	// Return the first object to match the filter
	template <typename O>
	O* first(Object::Type filter);

	// Return all object matching the filter
	template <typename O>
	std::vector<O*> all(Object::Type filter);

	// Tell the objects to update and remove those flagged for deletion
	void update();

	// Tell the objects to render
	void render();

	// Return the object list
	std::vector<Object*> objects() { return objects_; }
};

// Template access functions
template <typename O>
O* Manager::at(unsigned int i)
{
	if (i < objects_.size())
		return static_cast<O*>(objects_[i]);
	return nullptr;
}

template <typename O>
O* Manager::first(Object::Type filter)
{
	for (unsigned int i(0); i < objects_.size(); i++)
		if (objects_[i]->type() == filter)
			return static_cast<O*>(objects_[i]);
	return nullptr;
}

template <typename O>
std::vector<O*> Manager::all(Object::Type filter)
{
	std::vector<O*> found;

	for (unsigned int i(0); i < objects_.size(); i++)
		if (objects_[i]->type() == filter)
			found.push_back(static_cast<O*>(objects_[i]));
	return found;
}

} //namespace hc

#endif