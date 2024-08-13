#pragma once

#include "ShmingoApp.h"
#include <ShmingoCore.h>

#include "Matrices.h"

namespace Shmingo {
	void callDeferredSpacingRecalculation();


	//Key callback function
	void setGLFWkeyCallback();

	//Mouse button callback function
	void setGLFWMouseButtonCallback();

	//Mouse callback function
	void setGLFWWindowCallbacks();

	//Window callback function
	void setGLFWCursorPosCallback();
}