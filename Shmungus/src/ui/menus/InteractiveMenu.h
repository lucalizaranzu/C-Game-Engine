#pragma once

#include <ShmingoCore.h>

#include "InstancedVertexArray.h"
#include "MenuButton.h"

class InteractiveMenu {

public:

	InteractiveMenu();

	void init();

	void update();

	void cleanUp();

	void submitButton(MenuButton button);

protected:

	std::shared_ptr<TexturedQuadVertexArrayAtlas> m_vertexArray;

	std::vector<MenuButton> m_Buttons;

};