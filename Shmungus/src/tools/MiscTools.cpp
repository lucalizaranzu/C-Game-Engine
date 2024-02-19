#include "MiscTools.h"

//Disables cursor in order to use first person camera
void disableGLFWCursor(){

	glfwSetInputMode(se_application.getWindow()->getGLFWwindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

}

//Re enables normal cursor behavior
void enableGLFWCursor(){

	glfwSetInputMode(se_application.getWindow()->getGLFWwindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);

}
