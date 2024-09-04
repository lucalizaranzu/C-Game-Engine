#include <sepch.h>
#include "Player.h"
#include "ShmingoApp.h"

float roundToNearestThird(float num);

struct MovementTable {

	bool W;
	bool A;
	bool S;
	bool D;
	bool SPACE;
	bool LCTRL;
};

vec3 targetVelocity = vec3(0.0f, 0.0f, 0.0f);
MovementTable movementTable = MovementTable(false,false,false,false,false,false);

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

	se_application.setApplicationInfo(Shmingo::PLAYER_X, std::to_string(position.x).substr(0,10));
	se_application.setApplicationInfo(Shmingo::PLAYER_Y, std::to_string(position.y).substr(0, 10));
	se_application.setApplicationInfo(Shmingo::PLAYER_Z, std::to_string(position.z).substr(0, 10));

	se_application.setApplicationInfo(Shmingo::PLAYER_VELOCITY_X, std::to_string(velocity.x).substr(0, 10));
	se_application.setApplicationInfo(Shmingo::PLAYER_VELOCITY_Y, std::to_string(velocity.y).substr(0, 10));
	se_application.setApplicationInfo(Shmingo::PLAYER_VELOCITY_Z, std::to_string(velocity.z).substr(0, 10));

	se_uniformBuffer.setViewMatrix(Shmingo::createViewMatrix(camera));
}
 
//Sets flag to 1 and sets acceleration on key press
void Player::getKeyDown(KeyPressEvent* e) {

	if (e->getKey() == se_KEY_W) {
		targetVelocity.z = -MAX_SPEED;
		movementTable.W = true;
		if (movementTable.S == true) {
			targetVelocity.z = 0;
		}
	}
	else if (e->getKey() == se_KEY_A) {
		targetVelocity.x = -MAX_SPEED;
		movementTable.A = true;
		if (movementTable.D == true) {
			targetVelocity.x = 0;
		}
	}
	else if (e->getKey() == se_KEY_S) {
		targetVelocity.z = MAX_SPEED;
		movementTable.S = true;
		if (movementTable.W == true) {
			targetVelocity.z = 0;
		}
	}
	else if (e->getKey() == se_KEY_D) {
		targetVelocity.x = MAX_SPEED;
		movementTable.D = true;
		if (movementTable.A == true) {
			targetVelocity.x = 0;
		}
	}
	else if (e->getKey() == se_KEY_SPACE) {
		targetVelocity.y = MAX_SPEED;
		movementTable.SPACE = true;
		if (movementTable.LCTRL == true) {
			targetVelocity.y = 0;
		}
	}
	else if (e->getKey() == se_KEY_LEFT_CONTROL) {
		targetVelocity.y = -MAX_SPEED;
		movementTable.LCTRL = true;
		if (movementTable.SPACE == true) {
			targetVelocity.y = 0;
		}
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
		movementTable.W = false;
		targetVelocity.z = 0;
		if (movementTable.S == true) {
			targetVelocity.z = MAX_SPEED;
		}
	}
	if (e->getKey() == se_KEY_A) {
		movementTable.A = false;
		targetVelocity.x = 0;
		if (movementTable.D == true) {
			targetVelocity.x = MAX_SPEED;
		}
	}
	if (e->getKey() == se_KEY_S) {
		movementTable.S = false;
		targetVelocity.z = 0;
		if (movementTable.W == true) {
			targetVelocity.z = -MAX_SPEED;
		}
	}
	if (e->getKey() == se_KEY_D) {
		movementTable.D = false;
		targetVelocity.x = 0;
		if (movementTable.A == true) {
			targetVelocity.x = -MAX_SPEED;
		}
	}
	if (e->getKey() == se_KEY_SPACE) {
		movementTable.SPACE = false;
		targetVelocity.y = 0;
		if (movementTable.LCTRL == true) {
			targetVelocity.y = -MAX_SPEED;
		}
	}
	if (e->getKey() == se_KEY_LEFT_CONTROL) {
		movementTable.LCTRL = false;
		targetVelocity.y = 0;
		if (movementTable.SPACE == true) {
			targetVelocity.y = MAX_SPEED;
		}
	}
	else {
		return;
	}

	e->setHandled(); //Stops propogation if key was one we're looking for here
}

void Player::calcVelocity() {

	if (velocity.x - targetVelocity.x == 0) {
		//Do nothing
	}
	else if (velocity.x - targetVelocity.x > 0) { //Slowing down
		if (velocity.x - DEFAULT_ACCELERATION >= targetVelocity.x) {
			velocity.x -= DEFAULT_ACCELERATION;
		}
		else {
			velocity.x = targetVelocity.x;
		}
	}
	else { //Speeding up
		if (velocity.x + DEFAULT_ACCELERATION <= targetVelocity.x) {
			velocity.x += DEFAULT_ACCELERATION;
		}
		else {
			velocity.x = targetVelocity.x;
		}
	}
	//X calculations
	if (velocity.z - targetVelocity.z == 0) {
		//Do nothing
	}
	else if (velocity.z - targetVelocity.z > 0) { //Slowing down
		if (velocity.z - DEFAULT_ACCELERATION >= targetVelocity.z) {
			velocity.z -= DEFAULT_ACCELERATION;
		}
		else {
			velocity.z = targetVelocity.z;
		}
	}
	else { //Speeding up
		if (velocity.z + DEFAULT_ACCELERATION <= targetVelocity.z) {
			velocity.z += DEFAULT_ACCELERATION;
		}
		else {
			velocity.z = targetVelocity.z;
		}
	}
	//Y calculations
	if (velocity.y - targetVelocity.y == 0) {
		//Do nothing
	}
	else if (velocity.y - targetVelocity.y > 0) { //Slowing down
		if (velocity.y - DEFAULT_ACCELERATION >= targetVelocity.y) {
			velocity.y -= DEFAULT_ACCELERATION;
		}
		else {
			velocity.y = targetVelocity.y;
		}
	}
	else { //Speeding up
		if (velocity.y + DEFAULT_ACCELERATION <= targetVelocity.y) {
			velocity.y += DEFAULT_ACCELERATION;
		}
		else {
			velocity.y = targetVelocity.y;
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