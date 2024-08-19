#include <sepch.h>

#include "TextVertexArray.h"
#include "ShmingoApp.h"
#include "DataStructures.h"

//This is the worst code in this project as of 8/3/2024, please do not touch

TextVertexArray::TextVertexArray(std::string fontName) : fontName(fontName){

	//Vertex data for a quad
	GLfloat positions[] = {
			0.0f,0.0f,
			0.0f,1.0f,
			1.0f,1.0f,
			0.0f,0.0f,
			1.0f,0.0f,
			1.0f,1.0f
	};

	//Texture coordinates for a quad
	GLfloat texCoords[] = {
		0.0f, 0.0f, // bottom-left
		0.0f, 1.0f, // top-left
		1.0f, 1.0f, // top-right
		0.0f, 0.0f, // bottom-left
		1.0f, 0.0f, // bottom-right
		1.0f, 1.0f  // top-right
	};


	glGenVertexArrays(1, &vaoID); //Creates VAO using member variable

	glGenBuffers(1, &vertexPositionsVboID); //Generates a vertex buffer for positions
	glGenBuffers(1, &texCoordsVboID); //Generates a vertex buffer for texCoords

	glGenBuffers(1, &positionsVboID); //Generates a vertex buffer for positions
	glGenBuffers(1, &charDataVboID); //Generates a vertex buffer for charData (Containing texture ID, color, and scale)

	glBindVertexArray(vaoID); //Bind VAO

	//Set up vertex attributes

	//Static vertex attributes
	glBindBuffer(GL_ARRAY_BUFFER, vertexPositionsVboID);
	glBufferData(GL_ARRAY_BUFFER, 2 * 6 * sizeof(float), &positions, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, texCoordsVboID);
	glBufferData(GL_ARRAY_BUFFER, 2 * 6 * sizeof(float), &texCoords, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);

	//Per instance vertex attributes

	glBindBuffer(GL_ARRAY_BUFFER, positionsVboID);
	glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(float) * 1000, nullptr, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
	glVertexAttribDivisor(2, 1); //Per instance attribute

	glBindBuffer(GL_ARRAY_BUFFER, charDataVboID);
	glBufferData(GL_ARRAY_BUFFER, 3000, nullptr, GL_DYNAMIC_DRAW);

	glVertexAttribIPointer(3, 1, GL_UNSIGNED_BYTE, 3, (const void*)offsetof(Shmingo::GlyphData, charTextureID));
	glVertexAttribDivisor(3, 1); //Per instance attribute

	glVertexAttribIPointer(4, 1, GL_UNSIGNED_BYTE, 3, (const void*)offsetof(Shmingo::GlyphData, color));
	glVertexAttribDivisor(4, 1); //Per instance attribute

	glVertexAttribIPointer(5, 1, GL_UNSIGNED_BYTE, 3, (const void*)offsetof(Shmingo::GlyphData, scale));
	glVertexAttribDivisor(5, 1); //Per instance attribute

}

TextVertexArray::~TextVertexArray() {

}


void TextVertexArray::submitStaticText(TextBox& textBox){
	textBox.setVaoCharOffset(staticTextBoxEmplaceOffset); //Sets char offset to end of static text box section

	//Shift all dynamic text boxes to the right

	if (staticTextBoxEmplaceOffset != instanceAmount) {
		shiftBuffersRight(staticTextBoxEmplaceOffset, textBox.getTextBufferSize());
	}

	allocateSpaceForTextBox(&textBox);

	uploadTextBox(&textBox); //Populate the temp buffers and get offset of text box in buffer
	staticTextBoxEmplaceOffset += textBox.getTextBufferSize(); //Increment static text box offset

}

void TextVertexArray::submitDynamicText(DynamicTextBox& textBox){
	textBox.setVaoCharOffset(instanceAmount); //Sets char offset to end of static text box section

	allocateSpaceForTextBox(&textBox);

	uploadDynamicTextBox(&textBox);

	updateDynamicTextBox(textBox);
}

void TextVertexArray::updateDynamicTextBox(DynamicTextBox& textBox){

	reuploadDynamicTextBox(textBox, false);

	

}

void TextVertexArray::removeTextBox(GLuint offset, size_t textboxSize){
	instanceAmount -= textboxSize; //Decrements instance amount by size of text box
	shiftBuffersLeft(offset, textboxSize); //Shifts buffer data to the left by the size of the text box

}

void TextVertexArray::resetTextBox(TextBox& textBox){

	uploadTextBox(&textBox); //simple text box upload
}


void TextVertexArray::resetDynamicTextBox(DynamicTextBox& textBox){
	uploadDynamicTextBox(&textBox);
}


void TextVertexArray::reuploadDynamicTextBox(DynamicTextBox& textBox, bool reuploadAll){

	std::vector<std::string> sections = textBox.getSections(); //Get sections of text box

	bool reuploadFromCustomPosition = true;

	size_t textBoxOffset = textBox.getCharacterOffsetInVao(); //Index of current character in buffer
	size_t firstDynamicSectionIndex = textBox.getFirstDynamicSectionIndex(); //Index of first section in buffer

	size_t offsetInBuffer = textBoxOffset + textBox.getSectionBufferOffset(firstDynamicSectionIndex); //Index of current character in buffer

	float fontSize = (float)textBox.getFontSize();
	float lineSpacing = (float)textBox.getLineSpacing();
	vec2 boundingBox = textBox.getSize();
	vec2 position = textBox.getPosition();

	//Complicated way of getting line width from existing data without recalculating from scratch
	float yDistanceToDynamicSection = (position.y - textBox.getFirstDynamicSectionPointerPosition().y) * 39.0625f / fontSize; //Y position of first dynamic section relative to top of box converted to lines

	std::string::iterator c;

	float charX = position.x; //Can start from x position of first character we change
	float charY = (-1.0f * position.y) - (lineSpacing * (std::floor(yDistanceToDynamicSection) + 1)) * fontSize / 100.0f; //Y position of first dynamic section relative to top of box

	vec2 pointerPosition = vec2(charX, charY); //To pass as reference

	vec2 testPointerPosition = vec2(textBox.getPosition().x, textBox.getFirstDynamicSectionPointerPosition().y);

	size_t totalCharAmt = 0;

	std::string finalString;

	size_t currentTextboxSkipAmount = textBox.getTotalSkipAmount();
	textBox.resetTotalSkipAmount();
	for (size_t i = firstDynamicSectionIndex; i < sections.size(); i++) {

		std::string text = textBox.compileSection(i); //Get text of section

		finalString += text;
		
	}

	size_t reuploadOffset = textBoxOffset + textBox.getSectionBufferOffset(firstDynamicSectionIndex);

	if (currentTextboxSkipAmount != textBox.getTotalSkipAmount() || reuploadAll) {
		if (firstDynamicSectionIndex != 0) {
			uploadDynamicTextBox(&textBox); //Just reupload the whole thing to avoid conflict

			return;

		}
	}

	totalCharAmt = uploadTextToTempBuffers(finalString, textBox.getSectionBufferOffset(textBox.getFirstDynamicSectionIndex()), testPointerPosition, &textBox);

	alignTextInTempBuffers(textBox.getTextAlignment(), &textBox, reuploadFromCustomPosition);

	//se_log("Updating dynamic text box at gl buffer offset " << reuploadOffset << " with " << totalCharAmt << " characters");
	setGLBufferData(reuploadOffset, totalCharAmt);
}

void TextVertexArray::allocateSpaceForTextBox(TextBox* textBox){

	size_t textboxSize = textBox->getTextBufferSize(); //Get size of text box

	//se_log("Allocating space for text box at offset " << instanceAmount << " with size " << textboxSize);

	instanceAmount += textboxSize; //Increment instance amount
}


void TextVertexArray::uploadTextBox(TextBox* textBox) {

	vec2 position = vec2(textBox->getPosition().x, (-1.0f * textBox->getPosition().y) - textBox->getFontSize() / 100.0f);

	//Get information about textbox to avoid pointer chasing
	std::string text = textBox->getText();
	size_t offsetInBuffer = textBox->getCharacterOffsetInVao();
	vec2 pointerPosition = position; //To pass as reference

	size_t charAmt = uploadTextToTempBuffers(text, 0, pointerPosition, textBox);
	alignTextInTempBuffers(textBox->getTextAlignment(), textBox, false);

	//se_log("Uploading text box at gl buffer offset: " << offsetInBuffer);
	setGLBufferData(offsetInBuffer, charAmt);
	
}

void TextVertexArray::uploadDynamicTextBox(DynamicTextBox* textBox){
	if (textBox->isSectionDynamic(0)) {
		return; //Do nothing because the reupload function covers the entire text box
	}

	else {
		vec2 position = vec2(textBox->getPosition().x, (-1.0f * textBox->getPosition().y) - textBox->getFontSize() / 100.0f);

		//Get information about textbox to avoid pointer chasing
		textBox->resetTotalSkipAmount();

		std::string text = textBox->compileText(); //Upload only first section
		std::string firstSection = textBox->getSections()[0];

		size_t offsetInBuffer = textBox->getCharacterOffsetInVao();
		vec2 pointerPosition = position; //To pass as reference

		//Upload whole text box
		if (!textBox->isSectionDynamic(0)) {
			size_t resizeStartingOffset = uploadTextToTempBuffers(firstSection, 0, pointerPosition, textBox); //Upload first section
			textBox->setResizeStartingCharPointerPosition(pointerPosition); //Set starting position for resizing

			size_t firstDynamicSectionBufferOffset = textBox->getSectionBufferOffset(1); //Get offset of first dynamic section in buffer
			textBox->setFirstDynamicSectionPointerPosition(pointerPosition);
			textBox->setResizeStartingCharBufferOffset(resizeStartingOffset);
		}

		pointerPosition = position;

		size_t charAmt = uploadTextToTempBuffers(text, 0, pointerPosition, textBox); //Upload first section

		alignTextInTempBuffers(textBox->getTextAlignment(), textBox, false);

		//se_log("Uploading dynamic text box at gl buffer offset: " << offsetInBuffer);
		setGLBufferData(offsetInBuffer, charAmt);
	}
}

size_t TextVertexArray::uploadTextToTempBuffers(std::string text, size_t firstCharacterBufferOffset, vec2& pointerPosition, TextBox* textBox){
	size_t charAmt = 0;

	float resolutionScalingFactor = ((float)se_application.getWindow()->getHeight()) / ((float)se_application.getWindow()->getWidth());

	uint8_t defaultColor = textBox->getDefaultColor(); 
	uint8_t currentColorCode = defaultColor; //Default color is white

	vec2 startingPosition = textBox->getPosition();
	vec2 boundingBox = textBox->getSize();
	float lineSpacing = (float)textBox->getLineSpacing();
	float fontSize = (float)textBox->getFontSize();

	bool resetColor = true;

	vec2 lastPointerPosition = pointerPosition; 
	size_t offsetInBuffer = 0;

	size_t currentLine = 1;

	std::string::iterator c;

	//se_log("Text: " << text);

	for (c = text.begin(); c != text.end(); c++) {

		//Check for special characters
		if (*c == '\n') { //New line
			pointerPosition.y -= lineSpacing * fontSize / 256.25f; //Multiply by 256 to get pixel value
			pointerPosition.x = boundingBox.x; //Reset x position for new line

			currentColorCode = 0x3F;
			resetColor = true;
			continue;
		}
		else if (*c == ' ') {//Space character, avoid logic
			pointerPosition.x += resolutionScalingFactor * fontSize / 857.1428f; //Advance by space character width

			if (pointerPosition.x > (startingPosition.x + boundingBox.x)) {

				pointerPosition.y -= lineSpacing * fontSize / 256.25f; //Multiply by 256 to get pixel value
				pointerPosition.x = boundingBox.x; //Reset x position for new line

				textBox->setLineCharOffset(currentLine, offsetInBuffer + firstCharacterBufferOffset); //Set offset of new line

				currentLine++;

				continue;
			}

			currentColorCode = defaultColor;
			resetColor = true;
			continue;
		}
		else if (*c == '§') {
			char nextChar = *(c + 1); //Gets char after the § symbol and increments c
			if (nextChar == '§') {
				nextChar = *(c + 2);
				resetColor = false;
				c++;
			}

			try {
				currentColorCode = se_application.getTextColor(nextChar); //Get color code 
			}
			catch (const std::out_of_range& e) { se_error("Color code" << nextChar << "does not exist!"); }

			c++; //Increment iterator by one to avoid §x, since loop will automatically increment by one as well

			continue; //Skip remaining  part of loop
		}
		else if (*c == 'Ø') {
			markCharForSkip(offsetInBuffer, lastPointerPosition); //Skip character
			charAmt++; //Increment character amount because this will exist in the buffer
			offsetInBuffer++; //Increment offset
		}
		else {

			lastPointerPosition = pointerPosition; //Set last pointer position to current pointer position
			uploadCharacterToTempBuffers(*c, currentColorCode, offsetInBuffer, pointerPosition, textBox->getFontSize(), textBox->getLineSpacing(), boundingBox, startingPosition);
			offsetInBuffer++;
			if (resetColor) {
				currentColorCode = defaultColor; //Default color is white
			}
			charAmt++;
		}

		//Detect line overflow
		if (pointerPosition.y != lastPointerPosition.y) {

			textBox->setLineCharOffset(currentLine, offsetInBuffer + firstCharacterBufferOffset - 1); //Set offset of new line
			currentLine++;
		}
	}
	textBox->setLineCharOffset(currentLine, offsetInBuffer + firstCharacterBufferOffset); //Set offset of new line

	return charAmt;
}


void TextVertexArray::uploadCharacterToTempBuffers(char c, uint8_t colorCode, size_t offsetInBuffer, vec2& pointerPosition, GLuint fontSize, GLuint lineSpacing, vec2 boundingBox, vec2 startingPosition) {

	float resolutionScalingFactor = ((float)se_application.getWindow()->getHeight()) / ((float)se_application.getWindow()->getWidth());

	Shmingo::Character charInfo = se_application.getCharacterFontInfo(fontName, c); //Get glyph info about current character
	float totalCharSpace = (float)(charInfo.Advance) * fontSize / 10000.0f; //Total space taken up by a character

	//Fill to next line logic ---------------------------
	if (pointerPosition.x + totalCharSpace > (startingPosition.x + boundingBox.x)) { //Checks if line width is greater than text box width

		pointerPosition.y -= lineSpacing * fontSize / 156.25f; //Multiply by 256 to get pixel value
		pointerPosition.x = startingPosition.x; //Reset x position for new line
	}
	//---------------------------------------------------

	setCharPosition(vec2(pointerPosition.x + (fontSize * (float)charInfo.Bearing.x / 50000.0f),
		pointerPosition.y + (fontSize * (float)charInfo.Bearing.y / 50000.0f)),
		offsetInBuffer);
	setCharData(Shmingo::GlyphData(charInfo.TextureID, colorCode, (uint8_t)fontSize), offsetInBuffer);

	//se_log("Uploading character " << c << " at buffer offset " << offsetInBuffer);

	pointerPosition.x += resolutionScalingFactor * (float)(charInfo.Advance) * fontSize / 50000.0f;
	offsetInBuffer++; //Increment instance count

}


void TextVertexArray::alignTextInTempBuffers(Shmingo::TextAlignment alignment, TextBox* textBox, bool alignStartingFromResizePoint) {

	float resolutionScalingFactor = ((float)se_application.getWindow()->getHeight()) / ((float)se_application.getWindow()->getWidth());

	size_t lineBeginningOffset = textBox->getLineCharOffset(0); //Where to begin the shift

	float standardFontBearing = (float) se_application.getCharacterFontInfo(fontName, 'A').Bearing.x;

	float shiftAmount = 0;
	float endingCorrection = resolutionScalingFactor * textBox->getFontSize() * 35.0f / 10000.0f; //Amount to subtract to the shift since the final x value will be the beginning of the last character instead of the end

	float textboxEndPosition = textBox->getPosition().x + textBox->getSize().x;

	//In dynamic text boxes where we upload text starting from the first dynamic section, so it wont necessarily start from the beginning of a line
	if (alignStartingFromResizePoint) {

		lineBeginningOffset = textBox->getResizeStartingCharBufferOffset();

		size_t blueSample = textBox->getLineCharOffset(1) - textBox->getResizeStartingCharBufferOffset() - 1;

		float rightmostCharPosition = positionsTempBuffer[2 * blueSample];
		
		if (alignment == Shmingo::LEFT) {
			shiftAmount = textBox->getResizeStartingCharPointerPosition().x;
		}
		if (alignment == Shmingo::RIGHT) {
			shiftAmount = textboxEndPosition - rightmostCharPosition - endingCorrection;
		}
		else if (alignment == Shmingo::CENTER) {
			shiftAmount = (textboxEndPosition - rightmostCharPosition - endingCorrection - textBox->getResizeStartingCharPointerPosition().x) / 2.0f + textBox->getResizeStartingCharPointerPosition().x;
		}
	}

	else {
		float rightmostCharPosition = positionsTempBuffer[2 * textBox->getLineCharOffset(1) - 2];

		if (alignment == Shmingo::RIGHT) {
			shiftAmount = textboxEndPosition - rightmostCharPosition - endingCorrection;
		}
		else if (alignment == Shmingo::CENTER) {
			shiftAmount = (textboxEndPosition - rightmostCharPosition - endingCorrection) / 2.0f;
		}
	}


	size_t charsToShift = textBox->getLineCharOffset(1) - lineBeginningOffset;

	shiftPositionBufferValues(0, charsToShift, shiftAmount);

	for (size_t i = 1; i < textBox->getLineAmt(); i++) {

		size_t lineBeginningBufferPosition = textBox->getLineCharOffset(i);
		size_t lineEndingBufferPosition = textBox->getLineCharOffset(i + 1);

		float rightmostCharPosition = positionsTempBuffer[2 * lineEndingBufferPosition - 2];

		if (alignment == Shmingo::RIGHT) {
			shiftAmount = textboxEndPosition - rightmostCharPosition - endingCorrection;
		}

		else if (alignment == Shmingo::CENTER) {
			shiftAmount = (textboxEndPosition - rightmostCharPosition) / 2.0f;
		}

		shiftPositionBufferValues(lineBeginningBufferPosition, lineEndingBufferPosition - lineBeginningBufferPosition, shiftAmount);
	}
}



void TextVertexArray::cleanUp() {

	glDeleteBuffers(1, &vertexPositionsVboID);
	glDeleteBuffers(1, &texCoordsVboID);
	glDeleteBuffers(1, &positionsVboID);
	glDeleteBuffers(1, &charDataVboID);

	glDeleteVertexArrays(1, &vaoID);

	delete charDataTempBuffer;
	delete positionsTempBuffer;
}


void TextVertexArray::setCharData(Shmingo::GlyphData data, size_t offset) {
	charDataTempBuffer[offset] = data;
}

void TextVertexArray::setCharPosition(vec2 position, size_t charOffset) {
	positionsTempBuffer[2 * charOffset] = position.x;
	positionsTempBuffer[2 * charOffset + 1] = position.y;
}

void TextVertexArray::markCharForSkip(size_t offset, vec2 pointerPosition) {
	positionsTempBuffer[2 * offset] = pointerPosition.x;
	positionsTempBuffer[2 * offset + 1] = pointerPosition.y;

	charDataTempBuffer[offset] = Shmingo::GlyphData(0, 0x80, 0); //Sets unused color flag to 0b10000000, signaling the shaders to skip this character
}

void TextVertexArray::markRangeForSkip(size_t bufferOffset, size_t skipAmt, vec2 pointerPosition){
	for (int i = 0; i < skipAmt; i++) {
		markCharForSkip(bufferOffset + i, pointerPosition);
	}	
}

void TextVertexArray::setGLBufferData(size_t textBoxOffset, size_t charAmt) {

	bindVao();


	glBindBuffer(GL_ARRAY_BUFFER, charDataVboID);
	glBufferSubData(GL_ARRAY_BUFFER, textBoxOffset * 3, charAmt * 3, charDataTempBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, positionsVboID);
	glBufferSubData(GL_ARRAY_BUFFER, textBoxOffset * 2 * sizeof(float), 2 * charAmt * sizeof(float), positionsTempBuffer);
}


void TextVertexArray::setGLBufferDataPositionsOnly(size_t textBoxOffset, size_t charAmt) {
	glBindBuffer(GL_ARRAY_BUFFER, positionsVboID);
	glBufferSubData(GL_ARRAY_BUFFER, textBoxOffset * 2 * sizeof(float), 2 * charAmt * sizeof(float), positionsTempBuffer);
}

void TextVertexArray::shiftBuffersRight(size_t offset, size_t shiftAmt) {

	GLuint charAmt = instanceAmount - offset; //Amount of characters to shift

	bindVao();

	glBindBuffer(GL_ARRAY_BUFFER, positionsVboID);
	void* temp = glMapBufferRange(GL_ARRAY_BUFFER, offset * 2 * sizeof(float), charAmt * 2 * sizeof(float), GL_MAP_READ_BIT | GL_MAP_WRITE_BIT);
	if (temp) {
		memmove(static_cast<float*>(temp) + (shiftAmt * 2), temp, charAmt * 2 * sizeof(float));	//Perform the shift using memmove
		glUnmapBuffer(GL_ARRAY_BUFFER);	//Unmap the buffer after modification
	}

	glBindBuffer(GL_ARRAY_BUFFER, charDataVboID);
	temp = glMapBufferRange(GL_ARRAY_BUFFER, offset * 3, charAmt * 3, GL_MAP_READ_BIT | GL_MAP_WRITE_BIT);
	if (temp) {
		memmove(static_cast<Shmingo::GlyphData*>(temp) + shiftAmt, temp, charAmt * 3);	//Perform the shift using memmove
		glUnmapBuffer(GL_ARRAY_BUFFER);	//Unmap the buffer after modification
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void TextVertexArray::shiftBuffersLeft(size_t offset, size_t shiftAmt) {

	size_t charAmt = instanceAmount - offset + shiftAmt; //Amount of characters to shift

	bindVao();

	glBindBuffer(GL_ARRAY_BUFFER, positionsVboID);
	//Map starts where the leftmost change will happen
	void* temp = glMapBufferRange(GL_ARRAY_BUFFER, (offset) * 2 * sizeof(float), charAmt * 2 * sizeof(float), GL_MAP_READ_BIT | GL_MAP_WRITE_BIT);
	if (temp) {
		memmove(static_cast<float*>(temp), static_cast<float*>(temp) + (shiftAmt * 2), charAmt * 2 * sizeof(float));	//Perform the shift using memmove
		glUnmapBuffer(GL_ARRAY_BUFFER);	//Unmap the buffer after modification
	}

	glBindBuffer(GL_ARRAY_BUFFER, charDataVboID);
	temp = glMapBufferRange(GL_ARRAY_BUFFER, (offset) * 3, charAmt * 3, GL_MAP_READ_BIT | GL_MAP_WRITE_BIT);
	if (temp) {
		memmove(static_cast<Shmingo::GlyphData*>(temp), static_cast<Shmingo::GlyphData*>(temp) + (shiftAmt), charAmt * 3);	//Perform the shift using memmove

		glUnmapBuffer(GL_ARRAY_BUFFER);	//Unmap the buffer after modification
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void TextVertexArray::shiftPositionBufferValues(size_t offset, size_t charsToShift, float shiftAmt) {
	for (size_t bufferIndex = offset; bufferIndex < (offset + charsToShift); bufferIndex++) {
		positionsTempBuffer[2 * bufferIndex] += shiftAmt;
		//se_log("Shifted character " << charDataTempBuffer[bufferIndex].charTextureID << " by " << shiftAmt << " units");
		//std::cout << charDataTempBuffer[bufferIndex].charTextureID;
	}
	//std::cout << std::endl;
}


void TextVertexArray::printCharDataBuffer() {
	glBindBuffer(GL_ARRAY_BUFFER, charDataVboID);
	uint8_t* data = (uint8_t*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE); //maps buffer to memory
	for (GLuint i = 0; i < instanceAmount; i++) {
		se_log("Character " << (char)data[i * 3] << " is at offset " << i << ", with color code " << (int)data[i * 3 + 1]);
	}
	glUnmapBuffer(GL_ARRAY_BUFFER); //Unmaps buffer after reading
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}