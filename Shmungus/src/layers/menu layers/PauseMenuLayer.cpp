#include <sepch.h>

#include "Layer.h"
#include "LayerStack.h"

#include "MiscTools.h"

//Use this character to use color codes: §

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
	se_layerStack.addListener<PauseMenuLayer, MouseClickEvent>(Shmingo::PAUSEMENU_LAYER, this, &PauseMenuLayer::mouseClickCallBack);

	Shmingo::enableGLFWCursor(); //Re enable glfw cursor
	Shmingo::centerCursor();

	interactiveMenu->addTexture("menu_elements/unpressed.png");
	interactiveMenu->addTexture("menu_elements/pressed.png");
	interactiveMenu->completeTextureAtlas();

	int backButtonID = (int)interactiveMenu->createButton(vec2(0.35, 0.4), vec2(0.3, 0.066), "menu_elements/unpressed.png", "§§fBack", 8, 1);
	int closeButtonID = (int)interactiveMenu->createButton(vec2(0.35, 0.5), vec2(0.3, 0.066), "menu_elements/unpressed.png", "§§§PExit", 8, 1);

	interactiveMenu->setButtonOnClickFunction(backButtonID, []() {
		Shmingo::disableGLFWCursor(); //This should be thrown into a function in SandboxLayer or some kind of onSwitchToSandboxLayer function here, will do eventually
		se_layerStack.removeLayer(Shmingo::PAUSEMENU_LAYER);
		});
	interactiveMenu->setButtonOnClickFunction(closeButtonID, []() {
		se_application.setShoulApplicationClose();
		});
	interactiveMenu->setButtonOnMouseEnterFunction(backButtonID, [backButtonID]() {interactiveMenu->setButtonTexture(backButtonID, "menu_elements/pressed.png");});
	interactiveMenu->setButtonOnMouseExitFunction(backButtonID, [backButtonID]() {interactiveMenu->setButtonTexture(backButtonID, "menu_elements/unpressed.png");});
	interactiveMenu->setButtonOnMouseEnterFunction(closeButtonID, [closeButtonID]() {interactiveMenu->setButtonTexture(closeButtonID, "menu_elements/pressed.png"); });
	interactiveMenu->setButtonOnMouseExitFunction(closeButtonID, [closeButtonID]() {interactiveMenu->setButtonTexture(closeButtonID, "menu_elements/unpressed.png"); });
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
	//se_log("Cursor position: " << e->getXPos() << ", " << e->getYPos());
	//Simply a catcher for mouse drag events, should not do anything
	interactiveMenu->doButtonCollisionChecks(vec2(e->getXPos(), e->getYPos()));
	e->setHandled();

}

void PauseMenuLayer::mouseClickCallBack(MouseClickEvent* e) {

	int buttonIndex = interactiveMenu->doButtonHoverChecks();

	if (buttonIndex != -1) {
		if (e->getButton() == se_MOUSE_LEFT) {
			if (e->getAction() == se_PRESS) {
				interactiveMenu->setButtonIsMouseDown(buttonIndex, true); //Set button mouse down flag to true
			}
			else if (e->getAction() == se_RELEASE) {
				if (interactiveMenu->checkButtonIsMouseDown(buttonIndex)) {
					interactiveMenu->doButtonOnClick(buttonIndex);
					interactiveMenu->setButtonIsMouseDown(buttonIndex, false); //Set button mouse down flag to true
				}
			}
		}
	}
	else {
		if (e->getButton() == se_MOUSE_LEFT && e->getAction() == se_RELEASE){
			interactiveMenu->releaseAllButtons();
		}
	}

	e->setHandled();
}

void PauseMenuLayer::onDetach() {
	interactiveMenu->cleanUp();
}