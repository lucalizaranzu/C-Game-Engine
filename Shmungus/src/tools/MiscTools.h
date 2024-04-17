#pragma once

#include <ShmingoCore.h>
#include "ShmingoApp.h"
#include "World.h"

namespace Shmingo {
	void setCurrentWorld(World& world);
	void disableGLFWCursor();
	void enableGLFWCursor();
	void centerCursor();
}
