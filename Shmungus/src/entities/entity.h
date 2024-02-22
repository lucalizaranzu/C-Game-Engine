#pragma once

#include <ShmingoCore.h>

#include "Model.h"

class Entity{

public:

	Entity();

	//To be implemented
	void move();

	inline vec3 getPosition() { return position; };
	inline vec3 getRotation() { return rotation; };
	inline vec3 getDirection() { return direction; };

	inline Model getModel() { return model; }

protected:

	Model model;

	vec3 position;
	vec3 rotation; //No need to store a vector since we don't want entities rotating up or sideways
	vec3 direction;
};

