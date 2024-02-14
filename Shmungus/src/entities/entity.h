#pragma once

#include <ShmingoCore.h>

class Entity{

public:

	Entity();

	void move();

	inline vec3 getPosition() { return position; };
	inline vec2 getRotation() { return rotation; };

protected:

	//Will eventually include a model
	vec3 position;
	vec2 rotation; //No need to store a vector since we don't want entities rotating up or sideways
};

