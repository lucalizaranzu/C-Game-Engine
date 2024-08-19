#include <sepch.h>

#include "Layer.h"
#include "LayerStack.h"

//Use this character to use color codes: §

InfoLayer::InfoLayer() : infoSpace(InfoSpace()){
	type = Shmingo::INFO_LAYER;
}

void InfoLayer::onAttach() {
	se_layerStack.addListener<InfoLayer, KeyPressEvent>(Shmingo::INFO_LAYER, this, &InfoLayer::keybordCallback);

	infoSpace.submitDynamicTextBox(DynamicTextBox("FPS: ~§§ufps", vec2(0, 0.2), vec2(0.5f, 0.1f), 5, 1, 10, Shmingo::LEFT));
	infoSpace.submitDynamicTextBox(DynamicTextBox("Entity Count: ~§§dentityCount", vec2(0.5, 0.4), vec2(0.5f, 0.1f), 5, 1, 10, Shmingo::RIGHT));
	infoSpace.submitTextBox(TextBox("§§§PShmingus Engine static text aligned test, overflow please I dont want to have to maek this longer ok I have to make this longer to see how it interacts with more than one extra line thank you game", vec2(0, 0.3), vec2(1, 1), 5, 1, Shmingo::CENTER));
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
}