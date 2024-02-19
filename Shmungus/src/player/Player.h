#pragma once

#include <Engine.h>

#include "Entity.h"
#include "Camera.h"
#include "UniformBuffer.h"
#include "Matrices.h"

const float DEFAULT_ACCELERATION = 0.004f; //default acceleration of player
const float MAX_SPEED = 0.03f; //Max velocity of player
const float LOOK_SENSITIVITY = 0.002f;

class Player : public Entity {

	friend class Camera;

public:

	Player();
	void update();

private:

	Camera camera; //Player's camera, may free this if I want to do freecam not tied to player position eventually but who knows

	void getKeyDown(KeyPressEvent* e);
	void getKeyUp(KeyReleaseEvent* e);

	void getMouseMovement(MouseDragEvent* e);

	void calcVelocity();
	void move();

	vec3 velocity = vec3(0.0f,0.0f,0.0f);
	vec3 acceleration = vec3(0.0f, 0.0f, 0.0f);
};

