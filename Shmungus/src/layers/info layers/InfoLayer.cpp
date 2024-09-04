#include <sepch.h>

#include "Layer.h"
#include "LayerStack.h"

//Use this character to use color codes: §

InfoLayer::InfoLayer() : infoSpace(InfoSpace("Minecraft_Faithful")) {
	type = Shmingo::INFO_LAYER;
}

void InfoLayer::onAttach() {

	se_layerStack.addListener<InfoLayer, KeyPressEvent>(Shmingo::INFO_LAYER, this, &InfoLayer::keybordCallback);

	infoSpace.submitDynamicTextBox(DynamicTextBox("Entity Count: ~§§uentityCount", vec2(0.5, 0), vec2(0.5f, 0.1f), 5, 1, 10, Shmingo::RIGHT));
	infoSpace.submitDynamicTextBox(DynamicTextBox("Player Position: ~§§uplayerX, ~§§uplayerY, ~§§uplayerZ", vec2(0, 0.04f), vec2(1.0f, 0.1f), 5, 1, 10, Shmingo::LEFT));
	infoSpace.submitDynamicTextBox(DynamicTextBox("FPS: ~§§ufps", vec2(0, 0), vec2(0.2f, 0), 5, 1, 10, Shmingo::LEFT));
	infoSpace.submitDynamicTextBox(DynamicTextBox("Player Velocity: ~§§IplayerVelocityX, ~§§IplayerVelocityY, ~§§IplayerVelocityZ", vec2(0, 0.08f), vec2(1.0f, 0.1f), 5, 1, 10, Shmingo::LEFT));

	infoSpace.submitTextBox(TextBox("§§§FShmingo Engine property of §§mscrungly§F, all rights reserved", vec2(0, 0.97), vec2(1, 1), 4, 1, Shmingo::LEFT));
}

void InfoLayer::onDetach() {
	infoSpace.cleanUp();
}

void InfoLayer::onUpdate() {

	infoSpace.update();
}

void InfoLayer::keybordCallback(KeyPressEvent* e) {

	if (e->getKey() == se_KEY_F3) {

		se_layerStack.removeLayer(Shmingo::INFO_LAYER);
		e->setHandled();
	}
	else if (e->getKey() == se_KEY_F9) {
		infoSpace.printTextBuffer();
	}
}