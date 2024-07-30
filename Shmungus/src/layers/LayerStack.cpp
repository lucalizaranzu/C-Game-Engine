#include <sepch.h>
#include "LayerStack.h"


void LayerStack::init() {

	stack.emplace_back(new TopLayer());
	stack.back()->onAttach(); //Attaches the top layer

	layerInsertPosition = stack.begin();
	eventIterator = stack.rbegin();

}

LayerStack::~LayerStack() {

}

void LayerStack::emplaceLayer(Layer* layer) {
	//Adds layer to iterator position and increments iterator
	layerInsertPosition = stack.emplace(layerInsertPosition,layer); 
	eventIterator = stack.rbegin();

	layer->onAttach();
}

void LayerStack::emplaceOverlay(Layer* layer) {
	//Overlays are emplaced at the back, so no need for special iterator

	stack.emplace(stack.end() - 1, layer); //-1 because we always want the toplayer to be the last element in order to monitor events

	layerInsertPosition = stack.begin();
	eventIterator = stack.rbegin();

	layer->onAttach();
}

void LayerStack::removeLayer(Shmingo::LayerType layerType) {

	//Deletes layer from stack, may have to keep layer object intact in the future

	Layer* selectedLayer;

	//Iterates through stack and checks if type is same in order to select correct layer
	for (const auto& layer : stack) {
		if (layer->type == layerType) {
			selectedLayer = layer;
		}
	}

	stack.erase(find(stack.begin(), stack.end(), selectedLayer));

	layerInsertPosition = stack.begin();
	eventIterator = stack.rbegin();

	delete selectedLayer; //Remember this function can be called from inside the specified layer, which essentially is calling "delete this" - Be careful
}

void LayerStack::cleanUp(){
	for (Layer* layer : stack) {
		layer->onDetach();
		delete layer;
	}
	stack.clear();
}

void LayerStack::updateLayers() { //Can change this to use an update event in a tick-controlled environment

	for (const auto& layer : stack) {
		layer->onUpdate();
	}
}