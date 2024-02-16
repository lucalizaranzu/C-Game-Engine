#include "MiscTools.h"

void disableGLFWCursor(){

	glfwSetInputMode(se_application.getWindow()->getGLFWwindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

}
