#include "Layer.h"
#include "LayerStack.h"

#include "MiscTools.h"

PauseMenuLayer::PauseMenuLayer() {
	type = se_PAUSEMENU_LAYER;
}

void PauseMenuLayer::onAttach() {

	//Add new event listeners
	se_layerStack.addListener<PauseMenuLayer, KeyPressEvent>(se_PAUSEMENU_LAYER, this, &PauseMenuLayer::keybordCallback);
	se_layerStack.addListener<PauseMenuLayer, MouseDragEvent>(se_PAUSEMENU_LAYER, this, &PauseMenuLayer::mouseDragCallback);

	enableGLFWCursor(); //Re enable glfw cursor

	//TODO: re-center GLFW cursor to the center of the screen here
}


void PauseMenuLayer::onUpdate() {



}

void PauseMenuLayer::keybordCallback(KeyPressEvent* e) {

	if (e->getKey() == se_KEY_ESCAPE) {

		disableGLFWCursor(); //This should be thrown into a function in SandboxLayer or some kind of onSwitchToSandboxLayer function here, will do eventually
		se_layerStack.removeLayer(se_PAUSEMENU_LAYER); //Careful because this function deletes this pausemenulayer object, seems to work without issue though

	}

	e->setHandled(); //Don't pass any keyboard events to sandbox
}

void PauseMenuLayer::mouseDragCallback(MouseDragEvent* e){

	//Simply a catcher for mouse drag events, should not do anything

	e->setHandled();

}

void PauseMenuLayer::onDetach() {
}