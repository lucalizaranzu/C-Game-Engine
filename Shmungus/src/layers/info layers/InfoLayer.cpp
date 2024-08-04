#include <sepch.h>

#include "Layer.h"
#include "LayerStack.h"

InfoLayer::InfoLayer() : infoSpace(InfoSpace()){
	type = Shmingo::INFO_LAYER;
}

void InfoLayer::onAttach() {
	se_layerStack.addListener<InfoLayer, KeyPressEvent>(Shmingo::INFO_LAYER, this, &InfoLayer::keybordCallback);

	infoSpace.submitTextBox(TextBox("§fH§fe§fl§fl§fo, this will be a debug §As§bc§cr§De§Fe§Gn, I am putting as much text in here as possible to test the difference in FPS, Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat",vec2(0,0.5),vec2(2,2),8,1));
	infoSpace.submitDynamicTextBox(DynamicTextBox("Current FPS: ~fps frames per second", vec2(0, 0.5), vec2(2, 2), 6, 1));
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