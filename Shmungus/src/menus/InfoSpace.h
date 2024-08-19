#pragma once

#include <ShmingoCore.h>
#include "TextBox.h"
#include "TextVertexArray.h"


class InfoSpace {

public:

	InfoSpace(std::string fontName); //Default constructor
	InfoSpace(const InfoSpace& other); //Copy constructor
	

	void update(); //Update the info space


	void render(); //Render the info space

	void submitTextBox(TextBox textBox); //Submit a text box to the info space
	void removeTextBox(GLuint offset); //Remove a text box from the info space

	void submitDynamicTextBox(DynamicTextBox textBox); //Submit a dynamic text box to the info space
	void removeDynamicTextBox(GLuint offset); //Remove a dynamic text box from the info space
	void updateDynamicTextBoxes(); //Update all dynamic text boxes

	void recalculateTextSpacing(float oldDisplayWidth, float oldDisplayheight, float newDisplayWidth, float newDisplayHeight); //Resize all text boxes in the info space

	void printTextBuffer(); //Print the text buffer to the console

	void cleanUp();

private:

	GLuint m_applicationID; //Index in application list to identify the info space

	std::vector<TextBox> m_textBoxes; //List of text boxes
	std::vector<DynamicTextBox> m_dynamicTextBoxes; //List of dynamic text boxes



	std::shared_ptr<TextVertexArray> m_textVertexArray; //Text vertex array for rendering text

};