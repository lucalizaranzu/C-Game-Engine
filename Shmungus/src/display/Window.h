#pragma once
#include <ShmingoCore.h>

//This is a custom wrapper containing a GLFW window pointer, and any data associated with it
class Window {

private:

	int width;
	int height;
	GLFWwindow* window;

public:

	Window(int width, int height);

	void cleanUp();

	void setViewport();
	void setViewport(float red, float green, float blue, float alpha);

	//Wrappers for GLFW/Opengl functions to do with GLFWwindow
	int shouldDisplayClose();
	void swapBuffers();

	inline GLFWwindow* getGLFWwindow() { return window; }

};

//This is so we can create our own type of window instead of a GLFWwindow, macro defined in engine
Window* createWindow(int width, int height);