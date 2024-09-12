#include <sepch.h>
#include "Matrices.h"
#include "ShmingoApp.h"

mat4 Shmingo::createProjectionMatrix(GLfloat FOV, int displayWidth, int displayHeight, float nearPlane, float farPlane) {

	mat4 projectionMatrix = mat4(1.0f);

	//All of this will be added to the options class later
	projectionMatrix = glm::perspective(glm::radians(FOV), ((float)displayWidth / (float)displayHeight), nearPlane, farPlane);
	return projectionMatrix;
}

mat4 Shmingo::createViewMatrix(Camera camera){

	mat4 viewMatrix(1.0f); //Empty 4x4 matrix

	viewMatrix = glm::translate(viewMatrix, -camera.getPosition());

	//First argument is camera position, second argument is a 3D vector specifying the target, and the third argument is the up vector
	viewMatrix = glm::lookAt(camera.getPosition(), camera.getPosition() + calcDirection(camera.getRotation()), vec3(0, 1, 0));

	return viewMatrix;
}

mat4 Shmingo::createOrthoMatrix(int displayWidth, int displayHeight){
	mat4 projectionMatrix = mat4(1.0f);

	float aspect = (float)displayWidth / (float)displayHeight;

	//All of this will be added to the options class later
	projectionMatrix = glm::ortho(-aspect, aspect, -1.0f, 1.0f, 1.0f, -1.0f);
	return projectionMatrix;
}

mat4 Shmingo::createTransformationMatrix(vec3 position, vec3 rotation, vec3 scale){
	// Initialize the transformation matrix to the identity matrix
	glm::mat4 transformationMatrix = glm::mat4(1.0f);

	// Apply translation
	transformationMatrix = glm::translate(transformationMatrix, position);
	// Apply rotation: Yaw (around the Y axis) and Pitch (around the X axis)
	// First, rotate around the X axis (pitch)
	transformationMatrix = glm::rotate(transformationMatrix, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	transformationMatrix = glm::rotate(transformationMatrix, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	transformationMatrix = glm::rotate(transformationMatrix, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

	// Apply scaling
	transformationMatrix = glm::scale(transformationMatrix, scale);

	return transformationMatrix;

}
