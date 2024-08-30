#include <sepch.h>

#include "Layer.h"
#include "LayerStack.h"

#include "MiscTools.h"

PauseMenuLayer::PauseMenuLayer(){

	type = Shmingo::PAUSEMENU_LAYER;
}

std::shared_ptr<InteractiveMenu> interactiveMenu;

void PauseMenuLayer::onAttach() {

	interactiveMenu.reset(new InteractiveMenu());
	interactiveMenu->init();

	//Add new event listeners
	se_layerStack.addListener<PauseMenuLayer, KeyPressEvent>(Shmingo::PAUSEMENU_LAYER, this, &PauseMenuLayer::keybordCallback);
	se_layerStack.addListener<PauseMenuLayer, MouseDragEvent>(Shmingo::PAUSEMENU_LAYER, this, &PauseMenuLayer::mouseDragCallback);

	Shmingo::enableGLFWCursor(); //Re enable glfw cursor
	Shmingo::centerCursor();

	MenuButton button = MenuButton(vec2(0.2, 0.2), vec2(0.3, 0.15), 3);
	interactiveMenu->submitButton(button);
}


void PauseMenuLayer::onUpdate() {
	interactiveMenu->update();

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