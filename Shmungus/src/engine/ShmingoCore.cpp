#include "ShmingoCore.h"

void clearOpenGLError(){
	while (glGetError() != GL_NO_ERROR) {}
}

void checkOpenGLError(){
	while (GLenum error = glGetError()) {
		se_log("OpenGL Error:" << error)
	}
}
