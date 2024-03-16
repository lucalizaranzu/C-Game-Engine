#pragma once

#include <ShmingoCore.h>

#include "Layer.h"
#include "EventBus.h"

class LayerStack {

	friend class ShmingoApp;

public:

	inline static LayerStack& get() { return instance; };

	/// <summary>
	/// Throws an event of type type to all layers in the stack starting from the highest layer to the lowest.
	/// Will continue to propogate until a fucntion uses it which sets the handled flag
	/// </summary>
	/// <typeparam name="EventType">Type of event</typeparam>
	/// <param name="event">pointer to event</param>
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
	
	/// <summary>
	/// Adds a function pointer as a listener for a specific event type on a specific layer
	/// </summary>
	/// <typeparam name="InstanceClass">The class type which will have its member function be called on event</typeparam>
	/// <typeparam name="EventType">The event type</typeparam>
	/// <param name="layerType">Enum corresponding to layers, look at Layer.h to see options</param>
	/// <param name="instance">reference to the instance of the class whose member function will be called</param>
	/// <param name="instanceFunction">Function pointer to member function of instance class</param>
	template<class InstanceClass, class EventType>
	void addListener(LayerType layerType, InstanceClass* instance, void (InstanceClass::* instanceFunction)(EventType*)) {

		for (Layer* layer : stack) {
			if (layer->type == layerType) {
				layer->eventBus.subscribe(instance, instanceFunction); //Adds function to this layer's event bus
				return;
			}
		}
	}

	//Emplace a layer at the beginning of the stack
	void emplaceLayer(Layer* layer);
	//Emplace a layer at the end of the normal layers section, and at the beginning of the overlays section
	void emplaceOverlay(Layer* layer);
	//Removes a layer from the stack
	void removeLayer(LayerType layerType);

private:

	//Dont call this please 
	LayerStack() {};
	~LayerStack();

	void init();

	void updateLayers();

	//Dont touch these from anywhere outside of this class implementation
	std::vector<Layer*> stack;
	std::vector<Layer*>::iterator layerInsertPosition; //Position where regular layers are emplaced
	std::vector<Layer*>::reverse_iterator eventIterator; //For propogating events backwards
	//We can just use the default iterator to use a range based for loop for update functions

	static LayerStack instance; //This is a singleton so we need to hold on to its reference
};
