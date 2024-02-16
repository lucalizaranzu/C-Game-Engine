#pragma once

#include <ShmingoCore.h>

class Entity{

public:

	Entity();

	void move();

	inline vec3 getPosition() { return position; };
	inline vec3 getRotation() { return rotation; };
	inline vec3 getDirection() { return direction; };

protected:

	//Will eventually include a model
	vec3 position;
	vec3 rotation; //No need to store a vector since we don't want entities rotating up or sideways
	vec3 direction;
};

