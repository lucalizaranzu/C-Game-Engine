#pragma once

#include <Engine.h>
#include "Input.h"

//This class is a singleton, there will only ever be one Application
class ShmingoApp {

public:

	~ShmingoApp();

	inline Window* getWindow() { return window; };

	inline static ShmingoApp& get() { return instance; };

	void run();

private:

	LayerStack layerStack;
	ShmingoApp(); //Constructor
	Window* window;

	void init();
	void update();

	static ShmingoApp instance;

};