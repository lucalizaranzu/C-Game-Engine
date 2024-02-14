#pragma once

#include <ShmingoCore.h>
#include "EventBus.h"

enum LayerType {

	se_SANDBOX_LAYER

};

class LayerStack;

class Layer {

	friend class LayerStack;

public:

	Layer(); //Takes in the layer stack it will be a part of
	virtual ~Layer();


	template<class T, class EventType>
	void addListener(T* instance, void (T::* instanceFunction)(EventType*)) {

		eventBus.subscribe(instance, instanceFunction); //Adds function to this layer's event bus
	}

protected:

	virtual void onAttach() = 0;
	virtual void onDetach() = 0;
	virtual void onUpdate() = 0;

	EventBus eventBus;

	LayerType type;
};


