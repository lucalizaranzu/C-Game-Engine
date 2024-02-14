#pragma once

#include <ShmingoCore.h>

#include "Layer.h"
#include "EventBus.h"

class LayerStack {

	friend class ShmingoApp;

public:

	inline static LayerStack& get() { return instance; };

	//Throws an event of any event type, use new here because all event handlers will delete the pointer
	template<class EventType>
	inline void throwEvent(EventType* event) {
		for (auto it = eventIterator; it != stack.rend(); it++) { //Reverse iterate across all layers starting from highest to lowest in the list
			(*it)->eventBus.pushEvent<EventType>(event); //Push event into each layer, calling all listeners
			if (event->isHandled() || it == stack.rend() - 1) { //Checks either for event being handled or if event has reached end of list
				delete event; //Deletes event memory
				return; //Doesn't propogate to other layers once event is marked as handled
			}
		}
	}
	//Adds a function pointer as a listener, specify which layer should listen for the event, the instance of the class that will respond to the event, and a member function of the class to be called on event
	template<class T, class EventType>
	void addListener(LayerType layerType, T* instance, void (T::* instanceFunction)(EventType*)) {

		for (Layer* layer : stack) {
			if (layer->type == layerType) {
				layer->eventBus.subscribe(instance, instanceFunction); //Adds function to this layer's event bus
				return;
			}
		}
	}


private:

	//Dont call this please 
	LayerStack() {};
	~LayerStack();

	void init();

	void emplaceLayer(Layer* layer);
	void emplaceOverlay(Layer* layer);
	void removeLayer(Layer* layer);

	void updateLayers();

	//Dont touch these from anywhere outside of this class implementation
	std::vector<Layer*> stack;
	std::vector<Layer*>::iterator layerInsertPosition; //Position where regular layers are emplaced
	std::vector<Layer*>::reverse_iterator eventIterator; //For propogating events backwards
	//We can just use the default iterator to use a range based for loop for update functions

	static LayerStack instance; //This is a singleton so we need to hold on to its reference
};
