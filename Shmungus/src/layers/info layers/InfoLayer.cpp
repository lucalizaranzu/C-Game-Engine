#include <sepch.h>

#include "Layer.h"
#include "LayerStack.h"

InfoLayer::InfoLayer() : infoSpace(InfoSpace()){
	type = Shmingo::INFO_LAYER;
}

void InfoLayer::onAttach() {
	se_layerStack.addListener<InfoLayer, KeyPressEvent>(Shmingo::INFO_LAYER, this, &InfoLayer::keybordCallback);

	infoSpace.submitTextBox(TextBox("§§§KShmingo engine indev : property of scrungly the mutt all rights reserved", vec2(0, 0), ivec2(2, 2), 4, 1));
	infoSpace.submitDynamicTextBox(DynamicTextBox("Current FPS: ~§§jfps", vec2(0, 0.035), vec2(2, 2), 5, 1, 10));
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