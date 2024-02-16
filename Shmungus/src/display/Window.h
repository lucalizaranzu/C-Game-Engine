#pragma once
#include <ShmingoCore.h>

//This is a custom wrapper containing a GLFW window pointer, and any data associated with it
class Window {

	friend class ShmingoApp;

public:

	Window(int width, int height);

	void cleanUp();

	void setViewport();
	void setViewport(float red, float green, float blue, float alpha);

	//Wrappers for GLFW/Opengl functions to do with GLFWwindow
	int shouldDisplayClose();

	inline GLFWwindow* getGLFWwindow() { return window; }

	inline int getWidth() { return width; };
	inline int getHeight() { return height; };

	void setDimensions(int newWidth, int newHeight);

private:

	int width;
	int height;
	GLFWwindow* window;

	void swapBuffers();

};

//This is so we can create our own type of window instead of a GLFWwindow, macro defined in engine
Window* createWindow(int width, int height);