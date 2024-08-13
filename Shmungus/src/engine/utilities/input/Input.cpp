#include <sepch.h>
#include "Input.h"


//Initialize last known cursor position - will eventually be either calculated or loaded
bool firstMouse = true;
double lastX, lastY;

//Forward declarations
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouseButton_callback(GLFWwindow* window, int button, int action, int mods);
void mouseDrag_callback(GLFWwindow* window, double x, double y);
void framebufferResizeCallback(GLFWwindow* window, int width, int height);


void Shmingo::setGLFWkeyCallback() {
	glfwSetKeyCallback(se_application.getWindow()->getGLFWwindow(), key_callback);
}

void Shmingo::setGLFWMouseButtonCallback(){
	glfwSetMouseButtonCallback(se_application.getWindow()->getGLFWwindow(), mouseButton_callback);
}

void Shmingo::setGLFWCursorPosCallback(){
	glfwSetCursorPosCallback(se_application.getWindow()->getGLFWwindow(), mouseDrag_callback);
}

void Shmingo::setGLFWWindowCallbacks(){
	glfwSetFramebufferSizeCallback(se_application.getWindow()->getGLFWwindow(), framebufferResizeCallback);
}


//Used for resizing framebuffer
//TODO: limit allowable aspect ratios (Disable dragging)
void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
	
	if(width == 0 || height == 0) return; //Prevent division by zero

	float oldWidth = (float)se_application.getWindow()->getWidth(); //Get old window dimensions a ratio is sometimes needed
	float oldHeight = (float)se_application.getWindow()->getHeight();

	se_application.getWindow()->setDimensions(width, height);
	se_uniformBuffer.setProjectionMatrix(Shmingo::createProjectionMatrix(45.0f, (float)width, (float)height, 0.1f, 100.0f));
	se_uniformBuffer.setOrthoMatrix(Shmingo::createOrthoMatrix((float)width, (float)height));

	se_application.recalculateTextSpacing(oldWidth, oldHeight, (float)width, (float)height);
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

void mouseButton_callback(GLFWwindow* window, int button, int action, int mods) {
	
	if (action == GLFW_RELEASE) {
		se_layerStack.throwEvent<MouseReleaseEvent>(new MouseReleaseEvent(button, action, mods));
	}
	if (action == GLFW_PRESS) {
		se_layerStack.throwEvent<MouseClickEvent>(new MouseClickEvent(button, action, mods));
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
