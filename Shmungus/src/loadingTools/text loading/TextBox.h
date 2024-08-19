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

	void setLineCharOffset(size_t lineIndex, size_t offset);

	size_t getLineCharOffset(size_t lineIndex) { return charOffsetsOfLines[lineIndex]; };
	size_t getLineAmt() { return charOffsetsOfLines.size() - 1; };

	void setResizeStartingCharPointerPosition(vec2 position) { resizeStartingCharPointerPosition = position; };
	vec2 getResizeStartingCharPointerPosition() { return resizeStartingCharPointerPosition; };

	void setResizeStartingCharBufferOffset(size_t offset) { resizeStartingCharBufferOffset = offset; };
	size_t getResizeStartingCharBufferOffset() { return resizeStartingCharBufferOffset; };

protected:

	std::string text;

	vec2 position; //Position of the top left corner of the text box
	vec2 size; //Text box dimensions

	GLuint fontSize; //Font size of the text
	GLuint lineSpacing; //Spacing between lines of text

	Shmingo::TextAlignment textAlignment; //Alignment of the text

	size_t characterOffsetInVao = 0; //Character offset in the VAO
	size_t textBufferSize = 0; //Size of the text buffer

	virtual void parseText(){}; //Empty because we need to call this in the base constructor in a derived class
	virtual void setAllOffsets(){}; //Empty because we need to call this in the base constructor in a derived class

	virtual void setTextBufferSize(); //Returns the size of the text buffer

	uint8_t defaultColor = 0x3F; //Default color of the text

	std::vector<size_t> charOffsetsOfLines; //Buffer offset of each individual line in a textbox

	vec2 resizeStartingCharPointerPosition = vec2(0.0f,0.0f); //Starting position of the character pointer when resizing the text box
	size_t resizeStartingCharBufferOffset = 0;

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

	std::string compileSection(GLuint sectionIndex);

	vec2 getFirstDynamicSectionPointerPosition() { return firstDynamicSectionPosition; };

	void setAllOffsets() override;
	void setFirstDynamicSectionPointerPosition(vec2 position) { firstDynamicSectionPosition = position; };

	bool isSectionDynamic(GLuint index); //Check for dynamic text in the sections
	uint8_t getMaxDynamicTextSize() { return maxDynamicTextSize; };

	std::string getText() override;
	size_t getSectionSize(GLuint index);

	std::vector<std::string> getSections() { return sections; };
	size_t getSectionBufferOffset(size_t index) { return sectionBufferOffsets[index]; };
	size_t getFirstDynamicSectionIndex() { return firstDynamicSectionIndex; };

	size_t getTotalSkipAmount() { return totalSkipAmount; };
	void resetTotalSkipAmount() { totalSkipAmount = 0; };

	bool getShouldCompletelyReupload() { return shouldCompletelyReupload; };
	void setShouldCompletelyReupload(bool value) { shouldCompletelyReupload = value; };


private:

	uint8_t maxDynamicTextSize; //Maximum length dynamic text can have (applies to ALL dynamic sections)

	size_t firstDynamicSectionIndex = 0; //Index of the first dynamic section

	vec2 firstDynamicSectionPosition = vec2(0, 0); //Position of the first dynamic section

	size_t totalSkipAmount = 0; //Total amount of characters to skip in the text (Used to detect changes for complete reupload);
	bool shouldCompletelyReupload = false; //Flag to completely reupload the text

	std::vector<std::string> sections; //Sections of the text that are updated independently

	std::vector<size_t> sectionBufferOffsets; //Offsets of the sections in the text without spaces, final element is index of the end of the text

	void parseText() override; //Parse the text into sections
	void setTextBufferSize() override; //Sets the size of the text buffer

};

