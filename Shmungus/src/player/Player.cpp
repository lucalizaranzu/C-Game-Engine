#include <sepch.h>
#include "Player.h"
#include "ShmingoApp.h"

float roundToNearestThird(float num);

short stoppingArray[3] = { 0,0,0 };
//Axis on which player is decelerating to 0
//Format is                X,Z,Y

short movementArray[6] = { 0,0,0,0,0,0 };
//Format is                D,S,A,W,SPACE,CTRL

//Initializes camera with all zeros, THIS IS TEMPORARY! Camera should eventually take in vector pointers so it can be attached to player
Player::Player(Model model) : Entity(model, vec3(0.0f,0.0f,0.0f), vec3(0.0f,0.0f,0.0f)), camera(Camera(&position, &rotation, &direction)){

	se_layerStack.addListener<Player, KeyPressEvent>(Shmingo::SANDBOX_LAYER, this, &Player::getKeyDown);
	se_layerStack.addListener<Player, KeyReleaseEvent>(Shmingo::SANDBOX_LAYER, this, &Player::getKeyUp);
	se_layerStack.addListener<Player, MouseDragEvent>(Shmingo::SANDBOX_LAYER, this, &Player::getMouseMovement);

}

void Player::update() {

	calcVelocity();
	direction = calcDirection(rotation);
	move();

	se_application.setApplicationInfo(Shmingo::PLAYER_X, std::to_string(position.x));
	se_application.setApplicationInfo(Shmingo::PLAYER_Y, std::to_string(position.y));
	se_application.setApplicationInfo(Shmingo::PLAYER_Z, std::to_string(position.z));

	se_uniformBuffer.setViewMatrix(Shmingo::createViewMatrix(camera));
}
 
//Sets flag to 1 and sets acceleration on key press
void Player::getKeyDown(KeyPressEvent* e) {

	if (e->getKey() == se_KEY_W) {
		acceleration.z = -DEFAULT_ACCELERATION;
		movementArray[3] = 1;
		if (movementArray[1] == 1) {
			acceleration.z = 0;
		}
		else { stoppingArray[1] = 0; }
	}
	else if (e->getKey() == se_KEY_A) {
		acceleration.x = -DEFAULT_ACCELERATION;
		movementArray[2] = 1;
		if (movementArray[0] == 1) {
			acceleration.x = 0;
		}
		else { stoppingArray[0] = 0; }
	}
	else if (e->getKey() == se_KEY_S) {
		acceleration.z = DEFAULT_ACCELERATION;
		movementArray[1] = 1;
		if (movementArray[3] == 1) {
			acceleration.z = 0;
		}
		else { stoppingArray[1] = 0; }
	}
	else if (e->getKey() == se_KEY_D) {
		acceleration.x = DEFAULT_ACCELERATION;
		movementArray[0] = 1;
		if (movementArray[2] == 1) {
			acceleration.x = 0;
		}
		else { stoppingArray[0] = 0; }
	}
	else if (e->getKey() == se_KEY_SPACE) {
		acceleration.y = DEFAULT_ACCELERATION;
		movementArray[4] = 1;
		if (movementArray[5] == 1) {
			acceleration.y = 0;
		}
		else { stoppingArray[2] = 0; }
	}
	else if (e->getKey() == se_KEY_LEFT_CONTROL) {
		acceleration.y = -DEFAULT_ACCELERATION;
		movementArray[5] = 1;
		if (movementArray[4] == 1) {
			acceleration.y = 0;
		}
		else {stoppingArray[2] = 0; }
	}
	else {
		return;
	}

	e->setHandled(); //Stops propogation if key was one we're looking for here

}

//Sets flag to 0 on key release
//To handle automatic deceleration, use something like acceleration = -(finalVelocity - 0) / 5
void Player::getKeyUp(KeyReleaseEvent* e) {

	if (e->getKey() == se_KEY_W) {
		movementArray[3] = 0;
		if (movementArray[1] == 1) {
			acceleration.z = DEFAULT_ACCELERATION;
			stoppingArray[1] = 0;
		}
	}
	else if (e->getKey() == se_KEY_A) {
		movementArray[2] = 0;
		if (movementArray[0] == 1) {
			acceleration.x = DEFAULT_ACCELERATION;
			stoppingArray[0] = 0;
		}
	}
	else if (e->getKey() == se_KEY_S) {
		movementArray[1] = 0;
		if (movementArray[3] == 1) {
			acceleration.z = -DEFAULT_ACCELERATION;
			stoppingArray[1] = 0;
		}
	}
	else if (e->getKey() == se_KEY_D) {
		movementArray[0] = 0;
		if (movementArray[2] == 1) {
			acceleration.x = -DEFAULT_ACCELERATION;
			stoppingArray[0] = 0;
		}
	}
	else if (e->getKey() == se_KEY_SPACE) {
		movementArray[4] = 0;
		if (movementArray[5] == 1) {
			acceleration.y = -DEFAULT_ACCELERATION;
			stoppingArray[2] = 0;
		}
	}
	else if (e->getKey() == se_KEY_LEFT_CONTROL) {
		movementArray[5] = 0;
		if (movementArray[4] == 1) {
			acceleration.y = DEFAULT_ACCELERATION;
			stoppingArray[2] = 0;
		}
	}
	else {
		return;
	}

	e->setHandled(); //Stops propogation if key was one we're looking for here
}

void Player::calcVelocity() {

	//Set first to avoid being set to 0
	velocity.x = roundToNearestThird(velocity.x + acceleration.x);
	velocity.y = roundToNearestThird(velocity.y + acceleration.y);
	velocity.z = roundToNearestThird(velocity.z + acceleration.z);

	//temporary, will remove when I do the whole direction thing
	if (abs(velocity.x) >= MAX_SPEED) { acceleration.x = 0; }
	if (abs(velocity.y) >= MAX_SPEED) { acceleration.y = 0; }
	if (abs(velocity.z) >= MAX_SPEED) { acceleration.z = 0; }

	//Checking for same sign (If both opposite movement keys are the same, decelerate to 0)
	if (movementArray[3] == movementArray[1]) {
		if (velocity.z == 0.0f) {
			acceleration.z = 0.0f;
		}
		else if (velocity.z > 0.0f) {
			acceleration.z = (0 - DEFAULT_ACCELERATION);
		}
		else if (velocity.z < 0) {
			acceleration.z = (0 + DEFAULT_ACCELERATION);
		}
		if (velocity.z != 0.0f) {
			stoppingArray[1] = 1;
		}
	}
	if (movementArray[2] == movementArray[0]) {
		if (velocity.x == 0.0f) {
			acceleration.x = 0.0f;
		}
		else if (velocity.x > 0.0f) {
			acceleration.x = (0 - DEFAULT_ACCELERATION);
		}
		else if (velocity.x < 0) {
			acceleration.x = (0 + DEFAULT_ACCELERATION);
		}
		if (velocity.x != 0.0f) {
			stoppingArray[0] = 1;
		}
	}
	if (movementArray[4] == movementArray[5]) {
		if (velocity.y == 0.0f) {
			acceleration.y = 0.0f;
		}
		else if (velocity.y > 0.0f) {
			acceleration.y = (0 - DEFAULT_ACCELERATION);
		}
		else if (velocity.y < 0) {
			acceleration.y = (0 + DEFAULT_ACCELERATION);
		}
		if (velocity.y != 0.0f) {
			stoppingArray[2] = 1;
		}
	}

	if ((abs(velocity.x) < DEFAULT_ACCELERATION) && (abs(velocity.x > STOPPING_SPEED))) {
		if (stoppingArray[0] = 1) {
			if (velocity.x < 0) {
				acceleration.x = STOPPING_SPEED;
			}
			else {
				acceleration.x = -1 * STOPPING_SPEED;
			}
		}
	}
	else if ((abs(velocity.z) < DEFAULT_ACCELERATION) && (abs(velocity.z > STOPPING_SPEED))) {
		if (stoppingArray[1] = 1) {
			if (velocity.z < 0) {
				acceleration.z = STOPPING_SPEED;
			}
			else {
				acceleration.z = -1 * STOPPING_SPEED;
			}
		}
	}
	else if ((abs(velocity.y) < DEFAULT_ACCELERATION) && (abs(velocity.y > STOPPING_SPEED))) {
		if (stoppingArray[2] = 1) {
			if (velocity.y < 0) {
				acceleration.y = STOPPING_SPEED;
			}
			else {
				acceleration.y = -1 * STOPPING_SPEED;
			}
		}
	}

}

void Player::getMouseMovement(MouseDragEvent* e) {

	rotation.x += e->getXOffset() * LOOK_SENSITIVITY;
	rotation.y += e->getYOffset() * LOOK_SENSITIVITY;

	if (rotation.y > 1.57f) {
		rotation.y = 1.57f;
	}
	if (rotation.y < -1.57f) {
		rotation.y = -1.57f;
	}

}




//Turns velocity into position change, useful for eventual animation code etc.
void Player::move() {

	position.z += se_deltaTime * (velocity.z * cos(rotation.x) - velocity.x * sin(rotation.x));
	position.y += se_deltaTime * velocity.y;
	position.x += se_deltaTime * (velocity.x * cos(rotation.x) + velocity.z * sin(rotation.x));
}

float roundToNearestThird(float num) {
	return std::round(num * 1000.0f) / 1000.0f;
}