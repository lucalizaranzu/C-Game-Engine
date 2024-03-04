#include <sepch.h>
#include "Matrices.h"
#include "ShmingoApp.h"

mat4 createProjectionMatrix(GLfloat FOV, float displayWidth, float displayHeight, float nearPlane, float farPlane) {

	mat4 projectionMatrix = mat4(1.0f);

	//All of this will be added to the options class later
	projectionMatrix = glm::perspective(glm::radians(FOV), (float)(displayWidth / displayHeight), nearPlane, farPlane);
	return projectionMatrix;
}

mat4 createViewMatrix(Camera camera){

	mat4 viewMatrix(1.0f); //Empty 4x4 matrix

	viewMatrix = glm::translate(viewMatrix, -camera.getPosition());

	//First argument is camera position, second argument is a 3D vector specifying the target, and the third argument is the up vector
	viewMatrix = glm::lookAt(camera.getPosition(), camera.getPosition() + calcDirection(camera.getRotation()), vec3(0, 1, 0));

	return viewMatrix;
}