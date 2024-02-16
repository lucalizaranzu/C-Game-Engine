#pragma once

#include <ShmingoCore.h>
#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "MathTools.h"

mat4 createProjectionMatrix(GLfloat FOV, float displayWidth, float displayHeight, float nearPlane, float farPlane);

mat4 createViewMatrix(Camera camera);