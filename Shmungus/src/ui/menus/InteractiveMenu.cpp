#include <sepch.h>

#include "InteractiveMenu.h"
#include "MasterRenderer.h"
#include "MiscTools.h"
#include "ShmingoApp.h"

InteractiveMenu::InteractiveMenu(){

	m_textureAtlas.reset((new Shmingo::TextureAtlas(1250, 1250, true)));

	m_elementVertexArray.reset(new TexturedQuadVertexArrayAtlas(m_textureAtlas)); //Initialize vertex array
	m_textVertexArray.reset(new TextVertexArray("Minecraft")); //Initialize text vertex array
}

void InteractiveMenu::init(){

}

void InteractiveMenu::completeTextureAtlas(){
	m_elementVertexArray->init();
}

void InteractiveMenu::update(){

	if (Shmingo::shouldDoWindowResizeFunctions()) {
		recalculateTextSpacing(se_application.getLastFrameWindowDimensions().x, se_application.getLastFrameWindowDimensions().y,
			se_application.getWindow()->getWidth(), se_application.getWindow()->getHeight()); //Recalculates text spacing of all info spaces
	}



	se_masterRenderer.submitInstancedVertexArray(m_elementVertexArray, se_MENU_SHADER);
	se_masterRenderer.submitTextVertexArray(m_textVertexArray, se_TEXT_SHADER);
}

void InteractiveMenu::cleanUp(){

	m_elementVertexArray->cleanUp();
	m_textVertexArray->cleanUp();

	for (int i = 0; i < m_Buttons.size(); i++) {
		delete m_Buttons[i];
	}
}

size_t InteractiveMenu::createButton(vec2 position, vec2 size, std::string texture, std::string text, GLuint fontSize, GLuint lineSpacing){
	MenuButton* button = new MenuButton(position, size, m_textureMap[texture], text, fontSize, lineSpacing);
	button->setVaoID(m_elementVertexArray->submitQuad(button->getQuad(), button->getTextureID()));

	m_textVertexArray->submitStaticText(button->getTextBox());
	m_Buttons.emplace_back(button);
	return m_Buttons.size() - 1;
}

void InteractiveMenu::addTexture(std::string texturePath){
	m_textureAtlas->addTexture(m_textureAtlas->getTextureCount(), texturePath);
	m_textureMap.emplace(texturePath, m_textureAtlas->getTextureCount() - 1);
}

void InteractiveMenu::doButtonCollisionChecks(vec2 mousePosition){
	for (auto& button : m_Buttons) {
		button->checkMouseHovered(mousePosition);
	}
}

int InteractiveMenu::doButtonHoverChecks(){
	for (size_t i = 0; i < m_Buttons.size(); i++) {
		if (m_Buttons[i]->isMouseHovered()) {
			return (int)i;
		}
	}
	return -1; //No button found
}

void InteractiveMenu::doButtonOnClick(int buttonIndex) {
	std::function<void()> onClickFunction = m_Buttons[buttonIndex]->getOnClickFunction();
	if (onClickFunction) { //Check if the function is not null
		onClickFunction();
	}
	else {
		se_error("No function assigned to button with ID " << buttonIndex);
	}
}

void InteractiveMenu::doButtonOnMouseEnter(int buttonIndex){
	m_Buttons[buttonIndex]->getOnMouseEnterFunction()();
}

void InteractiveMenu::doButtonOnMouseExit(int buttonIndex){
	m_Buttons[buttonIndex]->getOnMouseExitFunction()();
}

void InteractiveMenu::setButtonOnClickFunction(int buttonIndex, std::function<void()> onClickFunction){
	m_Buttons[buttonIndex]->setOnClickFunction(onClickFunction);
}

void InteractiveMenu::setButtonOnMouseEnterFunction(int buttonIndex, std::function<void()> onMouseEnterFunction){
	m_Buttons[buttonIndex]->setOnMouseEnterFunction(onMouseEnterFunction);
}

void InteractiveMenu::setButtonOnMouseExitFunction(int buttonIndex, std::function<void()> onMouseExitFunction){
	m_Buttons[buttonIndex]->setOnMouseExitFunction(onMouseExitFunction);
}

void InteractiveMenu::setButtonTexture(int buttonIndex, std::string texture){
	m_Buttons[buttonIndex]->setTextureID(m_textureMap[texture]);
	m_elementVertexArray->resubmitQuad(m_Buttons[buttonIndex]->getVaoID(), m_Buttons[buttonIndex]->getQuad(), m_Buttons[buttonIndex]->getTextureID());
}

void InteractiveMenu::releaseAllButtons(){
	for (auto& button : m_Buttons) {
		button->setIsMouseDown(false);
	}
}

bool InteractiveMenu::checkButtonIsMouseDown(int buttonIndex){
	return m_Buttons[buttonIndex]->isMouseDown();
}

void InteractiveMenu::setButtonIsMouseDown(int buttonIndex, bool isMouseDown) {
	m_Buttons[buttonIndex]->setIsMouseDown(isMouseDown);
}


void InteractiveMenu::recalculateTextSpacing(float oldDisplayWidth, float oldDisplayHeight, float newDisplayWidth, float newDisplayHeight){
	float resolutionScalingFactor = ((float)se_application.getWindow()->getHeight()) / ((float)se_application.getWindow()->getWidth());

	float widthChange = newDisplayWidth / oldDisplayWidth;
	float heightChange = newDisplayHeight / oldDisplayHeight;

	for (auto& button : m_Buttons) {

		Shmingo::Quad quad = button->getQuad();

		m_elementVertexArray->resubmitQuad(button->getVaoID(), button->getQuad(), button->getTextureID());
		m_textVertexArray->resetTextBox(button->getTextBox());
	}
}