#pragma once

#include <ShmingoCore.h>
#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "MathTools.h"

namespace Shmingo {
	mat4 createProjectionMatrix(GLfloat FOV, int displayWidth, int displayHeight, float nearPlane, float farPlane);

	mat4 createViewMatrix(Camera camera);

	mat4 createOrthoMatrix(int displayWidth, int displayHeight);

	mat4 createTransformationMatrix(vec3 translation, vec3 rotation, vec3 scale);
}