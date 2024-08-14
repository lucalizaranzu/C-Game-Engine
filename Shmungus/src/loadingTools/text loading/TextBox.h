#pragma once

#include <ShmingoCore.h>

class TextBox {

public:

	TextBox(std::string text, vec2 position, vec2 size, GLuint fontSize, GLuint lineSpacing, Shmingo::TextAlignment alignment);

	void setVaoCharOffset(size_t offset) { characterOffsetInVao = offset; };

	//Getters
	virtual std::string getText() { return text; }; //Returns text

	GLuint getFontSize() { return fontSize; }; //Returns font size
	GLuint getLineSpacing() { return lineSpacing; }; //Returns line spacing

	uint8_t getDefaultColor() { return defaultColor; }; //Returns default color of the text

	vec2 getSize() { return size; }; //Returns dimensions of text box TODO change to vec2
	vec2 getPosition() { return position; }; //Returns position of text box

	Shmingo::TextAlignment getTextAlignment() { return textAlignment; }; //Returns alignment of the text

	size_t getTextBufferSize() {return textBufferSize;} //Returns the size of the text in buffer
	size_t getCharacterOffsetInVao() { return characterOffsetInVao; }; //Returns the character offset in the VAO

protected:

	std::vector<std::string> lines; //Lines of text in the text box

	std::string text;

	vec2 position; //Position of the top left corner of the text box
	vec2 size; //Text box dimensions

	GLuint fontSize; //Font size of the text
	GLuint lineSpacing; //Spacing between lines of text

	Shmingo::TextAlignment textAlignment; //Alignment of the text

	size_t characterOffsetInVao = 0;
	size_t textBufferSize = 0; //Size of the text buffer

	virtual void parseText(){}; //Empty because we need to call this in the base constructor in a derived class
	virtual void setAllOffsets(){}; //Empty because we need to call this in the base constructor in a derived class

	virtual void setTextBufferSize(); //Returns the size of the text buffer

	uint8_t defaultColor = 0x3F; //Default color of the text

};

class DynamicTextBox : public TextBox {

public:
	
	/// <summary>
	/// Text box which can replace sections of text preceeded by a '~' with system information
	/// </summary>
	/// <param name="text">String of text</param>
	/// <param name="position">Position of the text box</param>
	/// <param name="size">Size of the text box</param>
	/// <param name="fontSize">Font size</param>
	/// <param name="lineSpacing">Spacing between lines</param>
	/// <param name="maxDynamicTextSize">Maximum length dynamic text can have (applies to ALL dynamic sections)</param>
	DynamicTextBox(std::string text, vec2 position, vec2 size, unsigned int fontSize, unsigned int lineSpacing, uint8_t maxDynamicTextSize, Shmingo::TextAlignment alignment);

	void updateDynamicText();
	std::string compileText();
	std::string compileTextAfterFirstDynamicSection();

	std::string compileSection(GLuint sectionIndex);

	vec2 getPointerPositionBeforeDynamicSection() { return pointerPositionBeforeDynamicText; };
	vec2 getStartingPointerPosition() { return startingPointerPosition; };

	void setAllOffsets() override;
	void setpointerPositionBeforeDynamicText(vec2 position) { pointerPositionBeforeDynamicText = position; };
	void setStartingPointerPosition(vec2 position) { startingPointerPosition = position; };

	bool isSectionDynamic(GLuint index); //Check for dynamic text in the sections
	uint8_t getMaxDynamicTextSize() { return maxDynamicTextSize; };

	std::string getText() override;
	size_t getSectionSize(GLuint index);
	uint8_t getDynamicSectionDefaultColor(GLuint index);

	std::vector<std::string> getSections() { return sections; };
	size_t getSectionBufferOffset(size_t index) { return sectionBufferOffsets[index]; };
	size_t getFirstDynamicSectionIndex() { return firstDynamicSectionIndex; };

private:

	uint8_t maxDynamicTextSize; //Maximum length dynamic text can have (applies to ALL dynamic sections)
	size_t firstDynamicSectionIndex = 0; //Index of the first dynamic section

	vec2 pointerPositionBeforeDynamicText = vec2(0,0); //Position of the pointer before dynamic text was added
	vec2 startingPointerPosition = vec2(0,0); //Position of the pointer when the text box was created

	std::vector<std::string> sections; //Sections of the text that are updated independently
	std::unordered_map<size_t,uint8_t> dynamicSectionColors; //Default colors of the dynamic sections

	std::vector<size_t> sectionBufferOffsets; //Offsets of the sections in the text without spaces, final element is index of the end of the text

	void parseText() override; //Parse the text into sections
	void setTextBufferSize() override; //Sets the size of the text buffer

};

