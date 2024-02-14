#pragma once

#include <Engine.h>

#include "Entity.h"
#include "Camera.h"
#include "UniformBuffer.h"
#include "Matrices.h"

const float DEFAULT_ACCELERATION = 0.2f; //default acceleration of player
const float MAX_SPEED = 1.0f; //Max velocity of player

class Player : public Entity {

	friend class Camera;

public:

	Player();
	void update();

private:

	Camera camera; //Player's camera, may free this if I want to do freecam eventually but who knows

	void getKeyDown(KeyPressEvent* e);
	void getKeyUp(KeyReleaseEvent* e);
	void calcVelocity();
	void move();

	vec3 velocity = vec3(0.0f,0.0f,0.0f);
	vec3 acceleration = vec3(0.0f, 0.0f, 0.0f);
};

