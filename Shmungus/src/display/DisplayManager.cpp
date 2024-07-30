#include <sepch.h>

#include "DisplayManager.h"
#include "Engine.h"

void initDisplay() {

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
}

GLFWwindow* createGLFWwindow(int width, int height) {

	GLFWwindow* window = glfwCreateWindow(width, height, "Shmingo Engine Indev", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create window, terminating program!\n";
		glfwTerminate();
		return NULL;
	}

	return window;
}

void setGLFWViewport(GLFWwindow* window, int width, int height) { //Default viewport setter, clears to white background

	glViewport(0, 0, width, height);

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);

}

void setGLFWViewport(GLFWwindow* window, int width, int height, float clearRed, float clearGreen, float clearBlue, float clearAlpha) { //Overload with variable clear colors, sets the default background image color of the frame

	glViewport(0, 0, width, height);

	glClearColor(clearRed, clearGreen, clearBlue, clearAlpha);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);

}

void windowCleanUp(GLFWwindow* window) {

	glfwDestroyWindow(window);
	glfwTerminate();
}