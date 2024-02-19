#include "Input.h"


//Initialize last known cursor position - will eventually be either calculated or loaded
bool firstMouse = true;
double lastX, lastY;

//Forward declarations
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouseDrag_callback(GLFWwindow* window, double x, double y);
void framebufferResizeCallback(GLFWwindow* window, int width, int height);


void setGLFWkeyCallback() {

	glfwSetKeyCallback(se_application.getWindow()->getGLFWwindow(), key_callback);

}

void setGLFWCursorPosCallback(){

	glfwSetCursorPosCallback(se_application.getWindow()->getGLFWwindow(), mouseDrag_callback);

}

void setGLFWWindowCallbacks(){

	glfwSetFramebufferSizeCallback(se_application.getWindow()->getGLFWwindow(), framebufferResizeCallback);

}


//Used for resizing framebuffer
//TODO: limit allowable aspect ratios (Disable dragging)
void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
	
	se_application.getWindow()->setDimensions(width, height);
	se_uniformBuffer.setProjectionMatrix(createProjectionMatrix(45.0f, (float)width, (float)height, 0.1f, 100.0f));
}

//Key callback for keybaord input
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	if (action == 0) {
		se_layerStack.throwEvent<KeyReleaseEvent>(new KeyReleaseEvent(key, scancode, action, mods));
	}
	if (action == 1){
		se_layerStack.throwEvent<KeyPressEvent>(new KeyPressEvent(key, scancode, action, mods));
	}
	if (action == 2) {
		se_layerStack.throwEvent<KeyRepeatEvent>(new KeyRepeatEvent(key, scancode, action, mods));
	}
}

//Mouse callback for mouse input
void mouseDrag_callback(GLFWwindow* window, double x, double y) {

	if (firstMouse) {
		lastX = (float)x;
		lastY = (float)y;
		firstMouse = false;
	}

    float xOffset = (float)lastX - (float)x; //Get difference between last and new known mouse values
    float yOffset = (float)lastY - (float)y;
    lastX = (float)x;
    lastY = (float)y;

	se_layerStack.throwEvent<MouseDragEvent>(new MouseDragEvent(xOffset, yOffset)); //Throw new event

}