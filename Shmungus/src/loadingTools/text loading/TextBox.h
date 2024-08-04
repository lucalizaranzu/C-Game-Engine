#pragma once

#include <ShmingoCore.h>

class TextBox {

public:

	TextBox(std::string text, vec2 position, ivec2 size, unsigned int fontSize, unsigned int lineSpacing);

	void setVaoCharOffset(GLuint offset) { characterOffsetInVao = offset; };

	//Getters
	virtual std::string getText() { return text; }; //Returns text
	unsigned int getFontSize() { return fontSize; }; //Returns font size
	unsigned int getLineSpacing() { return lineSpacing; }; //Returns line spacing

	ivec2 getSize() { return size; }; //Returns dimensions of text box
	vec2 getPosition() { return position; }; //Returns position of text box
	GLuint getCharacterOffsetInVao() { return characterOffsetInVao; }; //Returns the character offset in the VAO

protected:

	std::vector<std::string> lines; //Lines of text in the text box

	std::string text;

	vec2 position; //Position of the top left corner of the text box
	ivec2 size; //Text box dimensions
	unsigned int fontSize; //Font size of the text
	unsigned int lineSpacing; //Spacing between lines of text

	unsigned int characterOffsetInVao = 0;

};

class DynamicTextBox : public TextBox {

public:

	DynamicTextBox(std::string text, ivec2 position, ivec2 size, unsigned int fontSize, unsigned int lineSpacing);

	void updateDynamicText();
	std::string compileText();

	bool isSectionDynamic(std::string section); //Check for dynamic text in the sections

	std::string getText() override;

	std::vector<std::string> getSections() { return sections; };
	std::vector<size_t> getSectionOffsets() { return sectionOffsets; };
	std::vector<size_t> getSectionOffsetsNoSpaces() { return sectionOffsetsNoSpaces; };

private:

	std::vector<std::string> sections; //Sections of the text that are updated independently

	std::vector<size_t> sectionOffsets; //Offsets of the sections in the text
	std::vector<size_t> sectionOffsetsNoSpaces; //Offsets of the sections in the text without spaces

	
	void parseText(std::string text); //Parse the text into sections
};

