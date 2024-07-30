#pragma once

#include <ShmingoCore.h>

class TextBox {

public:

	TextBox(std::string text, vec2 position, ivec2 size, unsigned int fontSize, unsigned int lineSpacing);

	void setText(std::string text); //Set the text of the text box

	void setVaoCharOffset(GLuint offset) { characterOffsetInVao = offset; };

	//Getters
	std::string getText() { return text; };
	unsigned int getFontSize() { return fontSize; };
	unsigned int getLineSpacing() { return lineSpacing; };
	ivec2 getSize() { return size; };
	vec2 getPosition() { return position; };
	GLuint getCharacterOffsetInVao() { return characterOffsetInVao; };

protected:

	std::vector<std::string> lines; //Lines of text in the text box

	std::string text;

	vec2 position; //Position of the top left corner of the text box
	ivec2 size; //Text box dimensions
	unsigned int fontSize; //Font size of the text
	unsigned int lineSpacing; //Spacing between lines of text

	unsigned int characterOffsetInVao = 0;

	void convertTextToLines(std::string text); //Convert the text into lines that fit within the text box

};

class DynamicTextBox : public TextBox {

public:

	DynamicTextBox(std::string text, ivec2 position, ivec2 size, unsigned int fontSize, unsigned int lineSpacing) : TextBox("", position, size, fontSize, lineSpacing) {};

	void updateDynamicText();

private:

	std::vector<std::string> sections; //Sections of the text that are updated independently
	
	void parseText(std::string text); //Parse the text into sections
	bool isSectionDynamic(std::string section); //Check for dynamic text in the sections

};

