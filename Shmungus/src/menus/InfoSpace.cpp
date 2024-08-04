#include <sepch.h>

#include "InfoSpace.h"
#include "MasterRenderer.h"
#include "ShmingoApp.h"
#include "MiscTools.h"



InfoSpace::InfoSpace() : m_applicationID(se_application.getInfoSpaceAmount()) {

	m_textVertexArray.reset(new TextVertexArray("Minecraft")); //Initialize vertex array to use arial font (May change to a parameter later if necessary)
	se_application.addInfoSpace(this); //Add the info space to the application list
}

InfoSpace::InfoSpace(const InfoSpace& other) {
	m_textVertexArray = other.m_textVertexArray; //Copy the text vertex array
}

void InfoSpace::update() {

	if (Shmingo::isTimeMultipleOf(0.5)) {
		updateDynamicTextBoxes();
	}
	render();
}





void InfoSpace::render(){
	se_masterRenderer.submitTextVertexArray(m_textVertexArray, se_TEXT_SHADER); //Render the text vertex array
}

void InfoSpace::submitTextBox(TextBox textBox){
	m_textVertexArray->submitStaticText(textBox); //Submit the text box to the vertex array
	m_textBoxes.push_back(textBox); //Add the text box to the list
}

void InfoSpace::submitDynamicTextBox(DynamicTextBox textBox){
	m_textVertexArray->submitDynamicText(textBox); //Submit the text box to the vertex array
	m_dynamicTextBoxes.push_back(textBox); //Add the text box to the list
}

void InfoSpace::updateDynamicTextBoxes(){
	for (const auto dynamicTextbox : m_dynamicTextBoxes) {
		m_textVertexArray->updateDynamicTextBox(dynamicTextbox); //Update all dynamic text boxes
	}
}

void InfoSpace::recalculateTextSpacing(float oldDisplayWidth, float oldDisplayHeight, float newDisplayWidth, float newDisplayHeight){
	m_textVertexArray->recalculateSpacing(oldDisplayWidth, oldDisplayHeight, newDisplayWidth, newDisplayHeight); //Recalculate the spacing of the text boxes
}

void InfoSpace::cleanUp() {
	m_textVertexArray->cleanUp(); //Clean up the text vertex array
	se_application.removeInfoSpace(m_applicationID); //Remove the info space from the application list
}