#pragma once

#include <ShmingoCore.h>

#include "InstancedVertexArray.h"
#include "TextVertexArray.h"
#include "MenuButton.h"

class InteractiveMenu {

public:

	InteractiveMenu();

	void init();
	void completeTextureAtlas();

	void update();

	void cleanUp();

	size_t createButton(vec2 position, vec2 size, std::string texture, std::string text, GLuint fontSize, GLuint lineSpacing);
	void addTexture(std::string texturePath);

	void doButtonCollisionChecks(vec2 mousePosition);
	int doButtonHoverChecks(); //Returns -1 if no button is hovered, otherwise returns the index of the hovered button

	void doButtonOnClick(int buttonIndex);
	void doButtonOnMouseEnter(int buttonIndex);
	void doButtonOnMouseExit(int buttonIndex);

	void setButtonOnClickFunction(int buttonIndex, std::function<void()> onClickFunction);
	void setButtonOnMouseEnterFunction(int buttonIndex, std::function<void()> onMouseEnterFunction);
	void setButtonOnMouseExitFunction(int buttonIndex, std::function<void()> onMouseExitFunction);
	void setButtonTexture(int buttonIndex, std::string texture);

	void releaseAllButtons();

	bool checkButtonIsMouseDown(int buttonIndex);
	void setButtonIsMouseDown(int buttonIndex, bool isMouseDown);

protected:

	void recalculateTextSpacing(float oldDisplayWidth, float oldDisplayHeight, float newDisplayWidth, float newDisplayHeight);

	std::shared_ptr<TexturedQuadVertexArrayAtlas> m_elementVertexArray;
	std::shared_ptr<TextVertexArray> m_textVertexArray;
	std::shared_ptr<Shmingo::TextureAtlas> m_textureAtlas;

	std::vector<MenuButton*> m_Buttons;

	std::unordered_map<std::string, size_t> m_textureMap;

};