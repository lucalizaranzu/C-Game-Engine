#include "LayerStack.h"


void LayerStack::init() {

	layerInsertPosition = stack.begin();
	eventIterator = stack.rbegin();
}

LayerStack::~LayerStack() {

	//Deletes all layers when stack is destroyed, may be changed later if all layers need to be 
	//Destroyed to change levels etc
	for (const auto& layer : stack) {
		layer->onDetach();
		delete layer;
	}
}

void LayerStack::emplaceLayer(Layer* layer) {
	//Adds layer to iterator position and increments iterator
	layerInsertPosition = stack.emplace(layerInsertPosition,layer); 
	layerInsertPosition = stack.begin();
	eventIterator = stack.rbegin();

	layer->onAttach();
}

void LayerStack::emplaceOverlay(Layer* layer) {
	//Overlays are emplaced at the back, so no need for special iterator
	stack.emplace_back(layer);

	layerInsertPosition = stack.begin();
	eventIterator = stack.rbegin();

	layer->onAttach();
}

void LayerStack::removeLayer(Layer* layer) {
	//Removes from stack, but does not delete the layer
	stack.erase(find(stack.begin(), stack.end(), layer));

	layerInsertPosition = stack.begin();
	eventIterator = stack.rbegin();
}

void LayerStack::updateLayers() { //Can change this to use an update event in a tick-controlled environment

	for (const auto& layer : stack) {
		layer->onUpdate();
	}
}

