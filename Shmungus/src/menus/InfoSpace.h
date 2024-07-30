#pragma once

#include <ShmingoCore.h>
#include "TextBox.h"
#include "TextVertexArray.h"


class InfoSpace {

public:

	InfoSpace(); //Default constructor
	InfoSpace(const InfoSpace& other); //Copy constructor
	

	void render(); //Render the info space

	void submitTextBox(TextBox textBox); //Submit a text box to the info space
	void removeTextBox(GLuint offset); //Remove a text box from the info space

	void recalculateTextSpacing(float oldDisplayWidth, float oldDisplayheight, float newDisplayWidth, float newDisplayHeight); //Resize all text boxes in the info space


	void cleanUp();

private:

	GLuint m_applicationID; //Index in application list to identify the info space

	std::vector<TextBox> m_textBoxes; //List of text boxes



	std::shared_ptr<TextVertexArray> m_textVertexArray; //Text vertex array for rendering text

};