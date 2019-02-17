#ifndef OBJECT_H_
#define OBJECT_H_

namespace hc {

class Object {
public:
	// All the objects type (should be one per class) 
	// Note : this enum is also used to order the objects in the manager's array,
	// So Types nearer to the top will update() first, and render first().
	// The Types nearer to the bottom will render() over them.
	static const int TYPE_ID = 0;

private:
	// The type of the object
	int type_;

	// Destruction flag
	bool destroy_;

public:
	Object(int type = TYPE_ID);
	virtual ~Object();

	// The object's update function. Is called $FPS times per second in the game loop
	virtual void update() = 0;

	// The object's render function. Should only contain graphics draw code. This function
	// is only called when the game is running fast enough, so putting gameplay code here might
	// make the game misbehave / feel laggy
	virtual void render() = 0;

	// Return the objet type
	int type() { return type_; }

	// Return if the object is flagged for deletion
	bool destroy() { return destroy_; }

	// Flag an object for deleti
	// Note : this allows you to unset the flag, but there is no reason to do that */
	void destroy(bool destroy) { destroy_ = destroy; }
};

} //namespace hc

#endif