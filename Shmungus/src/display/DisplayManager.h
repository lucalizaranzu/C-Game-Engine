#pragma once
#include <sepch.h>

#include <ShmingoCore.h>

void initDisplay();

GLFWwindow* createGLFWwindow(int width, int height);

void setGLFWViewport(GLFWwindow* window, int width, int height);
void setGLFWViewport(GLFWwindow* window, int width, int height, float clearRed, float clearGreen, float clearBlue, float clearAlpha);

void windowCleanUp(GLFWwindow* window);
