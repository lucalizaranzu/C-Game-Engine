#include "Input.h"

void setGLFWkeyCallback() {

	glfwSetKeyCallback(se_application.getWindow()->getGLFWwindow(), key_callback);

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	if (action == 1){
		se_layerStack.throwEvent<KeyPressEvent>(new KeyPressEvent(key, scancode, action, mods));
	}
	if (action == 0) {
		se_layerStack.throwEvent<KeyReleaseEvent>(new KeyReleaseEvent(key, scancode, action, mods));
	}
	if (action == 2) {
		se_layerStack.throwEvent<KeyRepeatEvent>(new KeyRepeatEvent(key, scancode, action, mods));
	}
}