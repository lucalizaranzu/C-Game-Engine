#pragma once

#include <ShmingoCore.h>
#include "ShmingoApp.h"
#include "World.h"

namespace Shmingo {
	//Sets world as the current world
	void setCurrentWorld(World* world);
	//Hides the cursor
	void disableGLFWCursor();
	//Unhides the cursor
	void enableGLFWCursor();
	//Sets the cursor position to the center of the window
	void centerCursor();
	//Returns true if the elapsed time is exactly equal to variable seconds in seconds
	bool isTimeMultipleOf(double seconds);
}
