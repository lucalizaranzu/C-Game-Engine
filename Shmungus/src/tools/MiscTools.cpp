#include <sepch.h>
#include "MiscTools.h"
#include "ShmingoApp.h"


void Shmingo::setCurrentWorld(World* world){
	se_application.setCurrentWorld(world);
}

//Disables cursor in order to use first person camera
void Shmingo::disableGLFWCursor() {

	glfwSetInputMode(se_application.getWindow()->getGLFWwindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if (glfwRawMouseMotionSupported()) {
		glfwSetInputMode(se_application.getWindow()->getGLFWwindow(), GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	}
}

//Re enables normal cursor behavior
void Shmingo::enableGLFWCursor() {

	glfwSetInputMode(se_application.getWindow()->getGLFWwindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);

}

void Shmingo::centerCursor() {

	glfwSetCursorPos(se_application.getWindow()->getGLFWwindow(), se_application.getWindow()->getWidth() / 2, se_application.getWindow()->getHeight() / 2);

}

bool Shmingo::isTimeMultipleOf(double seconds) {
    double currentTime = se_application.getElapsedTime();
    double lastTime = se_application.getLastElapsedTime();

    // Check if the current time has moved to the next integer multiple of 'seconds'
    if (std::floor(currentTime / seconds) != std::floor(lastTime / seconds)) {
        return true;
    }
    return false;
}

bool Shmingo::shouldDoWindowResizeFunctions(){
    if (se_application.getShouldRecalculateTextSpacing()) {
        return true;
    }
    return false;
}
