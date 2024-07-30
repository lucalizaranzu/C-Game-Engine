#include <sepch.h>

#include "Layer.h"
#include "LayerStack.h"

InfoLayer::InfoLayer() : infoSpace(InfoSpace()){
	type = Shmingo::INFO_LAYER;
}

void InfoLayer::onAttach() {
	se_layerStack.addListener<InfoLayer, KeyPressEvent>(Shmingo::INFO_LAYER, this, &InfoLayer::keybordCallback);

	infoSpace.submitTextBox(TextBox("§fH§fe§fl§fl§fo, this will be a debug §As§bc§cr§De§Fe§Gn",vec2(0,0),vec2(2,2),8,1));
}

void InfoLayer::onDetach() {
	infoSpace.cleanUp();
}

void InfoLayer::onUpdate() {

	infoSpace.render();
}

void InfoLayer::keybordCallback(KeyPressEvent* e) {

	if (e->getKey() == se_KEY_F3) {

		se_layerStack.removeLayer(Shmingo::INFO_LAYER);
		e->setHandled();
	}
}