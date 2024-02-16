#pragma once
#include <ShmingoCore.h>

class Camera {

public:

	Camera(vec3* position, vec3* rotation, vec3* direction);

	void rotate();

	inline vec3 getPosition() { return *position; };
	inline vec3 getRotation() { return *rotation; };
	//inline vec3 getDirection() { return direction; };

private:
	
	//Cameras must be tied to an entity, a sole camera cannot exist
	vec3* position;
	vec3* rotation; //Only need pitch and yaw, not roll
	vec3* direction;

};
