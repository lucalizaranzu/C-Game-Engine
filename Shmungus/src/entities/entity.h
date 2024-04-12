#pragma once

#include <sepch.h>
#include <ShmingoCore.h>
#include "Model.h"

class Entity {

public:

	Entity(Model model, vec3 position, vec3 rotation) : model(model), position(position), rotation(rotation) {}

	vec3 getPosition() { return position; };
	vec3 getRotation() { return rotation; };
	vec3 getDirection() { return direction; };

	Model getModel() { return model; };

protected:

	vec3 position;
	vec3 rotation;
	vec3 direction = vec3(0.0f,0.0f,0.0f);

	Model model;

};