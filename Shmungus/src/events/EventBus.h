#pragma once

#include <typeindex>

#include "ShmingoCore.h"
#include "Event.h"
#include "EventHandler.h"

typedef std::vector<HandlerFunction*> HandlerList; //Represents a vector of event handlers of a certain event type

class EventBus {

    friend class LayerStack;
    friend class Layer;

public:

private:
    std::map<std::type_index, HandlerList*> handlerMap;

    template<class EventType>
    void pushEvent(EventType* event) {
        HandlerList* handlers = handlerMap[typeid(EventType)]; //Checking for listeners for type
        if (handlers == nullptr) {
            return; //Return if there are no listeners for the event
        }
        for (auto& handler : *handlers) {
            if (handler != nullptr) {
                handler->handle(event);
            }
        }
    }

    template<class Caller, class EventType>
    void subscribe(Caller* instance, void (Caller::* memberFunction)(EventType*)) {
        HandlerList* handlers = handlerMap[typeid(EventType)];

        //Making new entry in map
        if (handlers == nullptr) {
            handlers = new HandlerList();
            handlerMap[typeid(EventType)] = handlers;
        }

        handlers->emplace_back(new EventHandler<Caller, EventType>(instance, memberFunction));
    }
};