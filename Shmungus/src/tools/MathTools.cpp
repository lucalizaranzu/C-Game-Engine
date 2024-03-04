#include <sepch.h>
#include "MathTools.h"

vec3 calcDirection(vec3 rotation) {

	vec3 direction;

	mat4 ones(1.0f);
	mat4 xRotMatrix = glm::rotate(ones, rotation.x, vec3(0.0f, 1.0f, 0.0f)); //Rotates the direction vector according to camera's x rotation
	mat4 yRotMatrix = glm::rotate(ones, rotation.y, vec3(1.0f, 0.0f, 0.0f)); //Rotates the direction vector according to camera's y rotation

	glm::vec4 finalVec(xRotMatrix * yRotMatrix * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)); //All math requires vectors of length 4

	return vec3(finalVec.x, finalVec.y, finalVec.z); //Return first 3 values
}