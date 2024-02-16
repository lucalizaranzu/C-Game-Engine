#include "Camera.h"

Camera::Camera(vec3* position, vec3* rotation, vec3* direction) : position(position), rotation(rotation), direction(direction){

}


//Since these transforms are tied to the player, they won't need to be implemented until a third person camera / camera not tied to a player is needed
void Camera::rotate() {

}
