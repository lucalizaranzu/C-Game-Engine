#include <sepch.h>

#include "InfoSpace.h"
#include "MasterRenderer.h"
#include "ShmingoApp.h"
#include "MiscTools.h"



InfoSpace::InfoSpace(std::string fontName) : m_applicationID((GLuint)se_application.getInfoSpaceAmount()) {

	m_textVertexArray.reset(new TextVertexArray(fontName)); //Initialize vertex array to use arial font (May change to a parameter later if necessary)
	se_application.addInfoSpace(this); //Add the info space to the application list
}

InfoSpace::InfoSpace(const InfoSpace& other) {
	m_textVertexArray = other.m_textVertexArray; //Copy the text vertex array
}

void InfoSpace::update() {

	if (Shmingo::isTimeMultipleOf(0.02)) {
		updateDynamicTextBoxes();
	}
	
	if (Shmingo::shouldDoWindowResizeFunctions()) {
		recalculateTextSpacing(se_application.getLastFrameWindowDimensions().x, se_application.getLastFrameWindowDimensions().y, 
			se_application.getWindow()->getWidth(), se_application.getWindow()->getHeight()); //Recalculates text spacing of all info spaces
	}

	render();
}



void InfoSpace::render(){
	se_masterRenderer.submitTextVertexArray(m_textVertexArray, se_TEXT_SHADER); //Render the text vertex array
}

void InfoSpace::submitTextBox(TextBox textBox){

	TextBox& textBoxRef = textBox; //Get reference to the text box

	size_t bufferSize = textBoxRef.getTextBufferSize(); //Get the size of the text buffer

	m_textBoxes.push_back(textBoxRef); //Add the text box to the list
	m_textVertexArray->submitStaticText(textBoxRef); //Submit the text box to the vertex array

	//Shift all dynamic text boxes to the right
	for (DynamicTextBox& textBox : m_dynamicTextBoxes) {
		textBox.setVaoCharOffset(textBox.getCharacterOffsetInVao() + bufferSize); //Update the character offset in the VAO
	}
}

void InfoSpace::removeTextBox(GLuint offset){

	GLuint bufferOffset = m_textBoxes[offset].getCharacterOffsetInVao(); //Get the buffer offset of the text box
	size_t bufferSize = m_textBoxes[offset].getTextBufferSize(); //Get the size of the text buffer

	m_textVertexArray->removeTextBox(bufferOffset, bufferSize); //Remove the text box from the vertex array
	m_textBoxes.erase(m_textBoxes.begin() + offset); //Remove the text box from the list

	for (GLuint i = offset; i < m_textBoxes.size(); i++) {
		TextBox& currentTextBox = m_textBoxes[i]; //Gets reference to the current dynamic text box being updated
		currentTextBox.setVaoCharOffset(currentTextBox.getCharacterOffsetInVao() - bufferSize); //Update the character offset in the VAO
	}

	for (DynamicTextBox& textBox : m_dynamicTextBoxes) {
		textBox.setVaoCharOffset(textBox.getCharacterOffsetInVao() - bufferSize); //Update the character offset in the VAO
	}
}

void InfoSpace::submitDynamicTextBox(DynamicTextBox textBox){
	m_textVertexArray->submitDynamicText(textBox); //Submit the text box to the vertex array
	m_dynamicTextBoxes.push_back(textBox); //Add the text box to the list
}

void InfoSpace::removeDynamicTextBox(GLuint offset){
	GLuint bufferOffset = m_dynamicTextBoxes[offset].getCharacterOffsetInVao(); //Get the buffer offset of the text box
	size_t bufferSize = m_dynamicTextBoxes[offset].getTextBufferSize(); //Get the size of the text buffer

	m_textVertexArray->removeTextBox(bufferOffset, bufferSize); //Remove the text box from the vertex array
	m_dynamicTextBoxes.erase(m_dynamicTextBoxes.begin() + offset); //Remove the text box from the list

	for (GLuint i = offset; i < m_dynamicTextBoxes.size(); i++) {
		DynamicTextBox& currentDynamictextBox = m_dynamicTextBoxes[i]; //Gets reference to the current dynamic text box being updated
		currentDynamictextBox.setVaoCharOffset(currentDynamictextBox.getCharacterOffsetInVao() - bufferSize); //Update the character offset in the VAO
	}
}

void InfoSpace::updateDynamicTextBoxes(){
	for (auto& dynamicTextbox : m_dynamicTextBoxes) {
		m_textVertexArray->updateDynamicTextBox(dynamicTextbox); //Update all dynamic text boxes
	}
}



void InfoSpace::recalculateTextSpacing(float oldDisplayWidth, float oldDisplayHeight, float newDisplayWidth, float newDisplayHeight){
	for (auto& textBox : m_textBoxes) {
		m_textVertexArray->resetTextBox(textBox); //Recalculate the spacing of all text boxes
	}
	for (auto& dynamicTextBox : m_dynamicTextBoxes) {
		m_textVertexArray->resetDynamicTextBox(dynamicTextBox); //Recalculate the spacing of all dynamic text boxes
	}
}

void InfoSpace::printTextBuffer(){
	m_textVertexArray->printCharDataBuffer(); //Print the text buffer to the console
}

void InfoSpace::cleanUp() {
	m_textVertexArray->cleanUp(); //Clean up the text vertex array
	se_application.removeInfoSpace(m_applicationID); //Remove the info space from the application list
}