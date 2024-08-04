#include <sepch.h>
#include "Window.h"
#include "DisplayManager.h"

Shmingo::Window::Window(int width, int height) : width(width), height(height) {

	initDisplay(); //Initializes display (Hopefully this is only done once, and I wont have to move this to main

	this->window = createGLFWwindow(width, height);

	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

}

void Shmingo::Window::cleanUp() {
	windowCleanUp(window);
}
//Defaukt wrapper for setViewport, white background
void Shmingo::Window::setViewport() {
	setGLFWViewport(window, width, height);
}

//Wrapper for setviewport, overloaded with custom colors to clear the background to
void Shmingo::Window::setViewport(float red, float green, float blue, float alpha) {
	setGLFWViewport(window, width, height, red, green, blue, alpha);
}

//Wrappers for GLFW/Opengl functions to do with GLFWwindow
int Shmingo::Window::shouldDisplayClose() { return glfwWindowShouldClose(window); }

void Shmingo::Window::setDimensions(int newWidth, int newHeight){

	width = newWidth;
	height = newHeight;

}
void Shmingo::Window::swapBuffers() {
	glfwSwapBuffers(window); 
}


//This is so we can create our own type of window instead of a GLFWwindow, macro defined in engine

Shmingo::Window* Shmingo::createWindow(int width, int height){
	return new Shmingo::Window(width, height);
}
