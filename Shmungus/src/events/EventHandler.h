#pragma once
#include "Event.h"

class HandlerFunction {

public:

	void handle(Event* event) { func(event); };

protected:

	virtual void func(Event* event) = 0;

};


template<class Caller, class EventType>
class EventHandler : public HandlerFunction {

public:
    typedef void (Caller::*CallerFunction)(EventType*); 

    EventHandler(Caller* instance, CallerFunction callerFunction) : instance(instance), callerFunction(callerFunction) {}


    void func(Event* event) {
        (instance->*callerFunction)(static_cast<EventType*>(event));
    }
private:
    //Caller instance
    Caller* instance;

    //Function pointer to the function
    CallerFunction callerFunction;

};


