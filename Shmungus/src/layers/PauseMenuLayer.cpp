#include <sepch.h>

#include "Layer.h"
#include "LayerStack.h"

#include "MiscTools.h"

PauseMenuLayer::PauseMenuLayer() {
	type = Shmingo::PAUSEMENU_LAYER;
}

void PauseMenuLayer::onAttach() {

	//Add new event listeners
	se_layerStack.addListener<PauseMenuLayer, KeyPressEvent>(Shmingo::PAUSEMENU_LAYER, this, &PauseMenuLayer::keybordCallback);
	se_layerStack.addListener<PauseMenuLayer, MouseDragEvent>(Shmingo::PAUSEMENU_LAYER, this, &PauseMenuLayer::mouseDragCallback);

	Shmingo::enableGLFWCursor(); //Re enable glfw cursor
	Shmingo::centerCursor();
}


void PauseMenuLayer::onUpdate() {



}

void PauseMenuLayer::keybordCallback(KeyPressEvent* e) {

	if (e->getKey() == se_KEY_ESCAPE) {

		Shmingo::disableGLFWCursor(); //This should be thrown into a function in SandboxLayer or some kind of onSwitchToSandboxLayer function here, will do eventually
		se_layerStack.removeLayer(Shmingo::PAUSEMENU_LAYER); //Careful because this function deletes this pausemenulayer object, seems to work without issue though

	}

	e->setHandled(); //Don't pass any keyboard events to sandbox
}

void PauseMenuLayer::mouseDragCallback(MouseDragEvent* e){

	//Simply a catcher for mouse drag events, should not do anything

	e->setHandled();

}

void PauseMenuLayer::onDetach() {
}