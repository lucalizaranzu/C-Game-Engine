#include <sepch.h>

#include "Layer.h"
#include "LayerStack.h"

TopLayer::TopLayer() {
	type = Shmingo::TOP_LAYER;
}

void TopLayer::onAttach() {

	se_layerStack.addListener<TopLayer, MouseClickEvent>(Shmingo::TOP_LAYER, this, &TopLayer::mouseClickCallback);
	se_layerStack.addListener<TopLayer, MouseReleaseEvent>(Shmingo::TOP_LAYER, this, &TopLayer::mouseReleaseCallback);

}

void TopLayer::onDetach() {

}

void TopLayer::onUpdate() {

}

void TopLayer::mouseClickCallback(MouseClickEvent* e) {
	if (e->getButton() == se_MOUSE_BUTTON_1) {
	}
}

void TopLayer::mouseReleaseCallback(MouseReleaseEvent* e) {

	if (e->getButton() == se_MOUSE_BUTTON_1) {
	}
}