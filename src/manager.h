#include <vector>

#include "object.h"

#ifndef MANAGER_H_
#define MANAGER_H_

class Manager {
private:
    std::vector<Object*> objects_;

public:
    Manager();
    ~Manager();

	/* Add an object to the list */
    void add(Object* object);

    /* Remove an object from the list */
    void remove(Object* object);

	/* Find the position in the array of the object */
    int find(Object* object);

	/* Return the first object to match the filter */
	Object* first(Object::Type filter);

	/* Return all object matching the filter */
    std::vector<Object*> all(Object::Type filter);

	/* Tell the objects to update and remove those flagged for deletion */
    void update();

	/* Tell the objects to render */
    void render();

	/* Return the object list */
    std::vector<Object*> objects() { return objects_; }
};

#endif