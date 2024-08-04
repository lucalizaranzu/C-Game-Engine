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
	glGenBuffers(1, &charTextureVboID); //Generates a vertex buffer for charTexture
	glGenBuffers(1, &colorVboID); //Generates a vertex buffer for color
	glGenBuffers(1, &positionsVboID); //Generates a vertex buffer for positions
	glGenBuffers(1, &scaleVboID); //Generates a vertex buffer for scale


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
	glBindBuffer(GL_ARRAY_BUFFER, charTextureVboID);
	glBufferData(GL_ARRAY_BUFFER, 2000, nullptr, GL_DYNAMIC_DRAW);
	glVertexAttribIPointer(2, 1, GL_UNSIGNED_BYTE, 0, nullptr);
	glVertexAttribDivisor(2, 1); //Per instance attribute

	glBindBuffer(GL_ARRAY_BUFFER, colorVboID);
	glBufferData(GL_ARRAY_BUFFER, 2000, nullptr, GL_DYNAMIC_DRAW);
	glVertexAttribIPointer(3, 1, GL_UNSIGNED_BYTE, 0, nullptr);
	glVertexAttribDivisor(3, 1); //Per instance attribute

	glBindBuffer(GL_ARRAY_BUFFER, positionsVboID);
	glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(float) * 2000, nullptr, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
	glVertexAttribDivisor(4, 1); //Per instance attribute

	glBindBuffer(GL_ARRAY_BUFFER, scaleVboID);
	glBufferData(GL_ARRAY_BUFFER, 2000, nullptr, GL_DYNAMIC_DRAW);
	glVertexAttribIPointer(5, 1, GL_UNSIGNED_BYTE,0, nullptr);
	glVertexAttribDivisor(5, 1); //Per instance attribute

}

TextVertexArray::~TextVertexArray() {

}


void TextVertexArray::submitStaticText(TextBox& textBox){

	textBox.setVaoCharOffset(staticTextBoxEmplaceOffset); //Sets char offset to end of static text box section

	GLuint charAmt = loadTempBuffers(&textBox); //Populate the temp buffers and get offset of text box in buffer

	GLuint textBoxOffset = textBox.getCharacterOffsetInVao(); //Get offset of text box in buffer

	setGLBufferData(textBoxOffset, charAmt); //Copies the temp buffers to the GL buffers

	instanceAmount += charAmt; //Increment instance (glyph) amount
	staticTextBoxEmplaceOffset += charAmt; //Increment offset for next static text box
}

void TextVertexArray::submitDynamicText(DynamicTextBox& textBox){

	textBox.setVaoCharOffset(instanceAmount); //Sets char offset to end of buffer
	GLuint charAmt = loadTempBuffers(&textBox); //Populate the temp buffers and get offset of text box in buffer
	GLuint textBoxOffset = textBox.getCharacterOffsetInVao(); //Get offset of text box in buffer

	setGLBufferData(textBoxOffset, charAmt); //Copies the temp buffers to the GL buffers

	instanceAmount += charAmt; //Increment instance (glyph) amount
}

void TextVertexArray::updateDynamicTextBox(DynamicTextBox textBox){

	std::vector<std::string> sections = textBox.getSections(); //Get sections of text box

	bool pastFirstDynamic = false; //Flag for if the first dynamic section has been passed
	size_t firstDynamicSectionIndex = 0;

	int sectionIndex = 0;
	while(!pastFirstDynamic) { //Loops until we hit the first dynamic section
		if (textBox.isSectionDynamic(sections[sectionIndex])) {
			pastFirstDynamic = true;
			firstDynamicSectionIndex = sectionIndex; //Gets offset of first dynamic section, we wont have to shift anything before this
			break;
		}
		sectionIndex++;
	}

	reuploadDynamicText(textBox, firstDynamicSectionIndex); //Reuploads dynamic text to buffer
}

void TextVertexArray::recalculateSpacing(float oldWidth, float oldHeight, float newWidth, float newHeight){

	bindVao();
	glBindBuffer(GL_ARRAY_BUFFER, positionsVboID);

	float* data = (float*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE); //maps buffer to memory

	GLint bufferSize;
	glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &bufferSize);

	float oldAspectRatio = oldWidth / oldHeight;
	float newAspectRatio = newWidth / newHeight;

	for (GLuint i = 0; i < instanceAmount; ++i) {
		float copy = data[i*2];
		data[i * 2] *= (oldAspectRatio / newAspectRatio); //Divides all x components by aspect ratio
		//data[(i * 2) + 1] *= (newHeight / oldHeight); //Divides all y components by aspect ratio
	}

	if (!glUnmapBuffer(GL_ARRAY_BUFFER)) {
		se_error("Couldn't unmap buffer after recalculating spacing");
	}

}

void TextVertexArray::cleanUp(){
	delete textureTempBuffer;
	delete colorTempBuffer;
	delete positionsTempBuffer;
	delete scaleTempBuffer;
}

void TextVertexArray::setCharTexture(uint8_t textureID, GLuint charOffset){
	textureTempBuffer[charOffset] = textureID;
}

void TextVertexArray::setCharSize(uint8_t size, GLuint charOffset){
	scaleTempBuffer[charOffset] = size;
}

void TextVertexArray::setCharColor(uint8_t colorCode, GLuint charOffset){
	colorTempBuffer[charOffset] = colorCode;
}

void TextVertexArray::setCharPosition(vec2 position, GLuint charOffset){
	positionsTempBuffer[2 * charOffset] = position.x;
	positionsTempBuffer[2 * charOffset + 1] = position.y;
}

void TextVertexArray::reuploadDynamicText(DynamicTextBox textBox, size_t firstSectionIndex){

	float resolutionScalingFactor = ((float)se_application.getWindow()->getHeight()) / ((float)se_application.getWindow()->getWidth());

	GLuint textBoxOffset = textBox.getCharacterOffsetInVao(); //Index of current character in buffer
	size_t bufferStartIndex = textBox.getSectionOffsetsNoSpaces()[firstSectionIndex];

	float fontSize = (float)textBox.getFontSize();
	float lineSpacing = (float)textBox.getLineSpacing();
	vec2 position = textBox.getPosition();

	size_t bufferSize = textBox.getSectionOffsetsNoSpaces()[firstSectionIndex] * 2 * sizeof(float);

	bindVao();
	glBindBuffer(GL_ARRAY_BUFFER, positionsVboID);
	float* positionsBuffer = static_cast<float*>(glMapBufferRange(GL_ARRAY_BUFFER, textBoxOffset * 2 * sizeof(float), bufferSize, GL_MAP_WRITE_BIT)); //Maps buffer to memory

	float startingX = positionsBuffer[0]; //X position of first character in buffer
	float startingY = positionsBuffer[1]; //Y position of first character in buffer

	//Complicated way of getting line width from existing data without recalculating from scratch
	float currentLineWidth = (positionsBuffer[2 * (bufferStartIndex) - 2] - startingX);
	float yDistanceToDynamicSection = (startingY - positionsBuffer[2 * (bufferStartIndex)-1]) * 39.0625f / fontSize; //Y position of first dynamic section relative to top of box converted to lines

	glUnmapBuffer(GL_ARRAY_BUFFER);

	//int startingLine = ();

	std::string::iterator c;

	float charX = startingX + currentLineWidth; //Can start from x position of first character we change
	float charY = (-1.0f * position.y) - (lineSpacing * (std::floor(yDistanceToDynamicSection) + 1)) * fontSize / 100.0f; //Y position of first dynamic section relative to top of box

	currentLineWidth = currentLineWidth * (5.0f / resolutionScalingFactor); //I dont fucking know why I have to do this after setting charX these things should be in the same units but theyre not

	GLuint charAmt = 0;
	GLuint totalCharAmt = 0;
	for (int i = firstSectionIndex; i < textBox.getSections().size(); i++) {

		std::string section = textBox.getSections().at(i); //Current section starting at first dynamic section
		if (textBox.isSectionDynamic(section)) {

			uint8_t currentColor = 0xFF; //Default color is white
			GLuint currentIndex = 0;

			std::string text = se_application.getApplicationInfo(section); //Get dynamic text

			if (firstSectionIndex > 0) {
				if (textBox.getSections()[firstSectionIndex - 1].back() == ' ') {
					charX += (float)(se_application.getCharacterFontInfo(' ').Advance) * fontSize / 66000.0f; //Advance by space character width
				}
			}


			for (c = text.begin(); c != text.end(); c++) { //Add 1 to skip the § symbol

				Shmingo::Character currentChar = se_application.getCharacterFontInfo(*c); //Get glyph info about current character
				float totalCharSpace = (float)(currentChar.Advance) * fontSize / 10000.0f; //Total space taken up by a character

				setCharColor(currentColor, charAmt);

				//Check for special characters
				if (*c == '\n') { //New line
					charY -= lineSpacing * 256.0f * fontSize / 10000.0f; //Multiply by 256 to get pixel value
					charX = position.x; //Reset x position for new line
					currentIndex++; //Increment index
					continue; //Skip remaining part of loop
				}
				else if (*c == ' ') {//Space character, avoid logic
					charX += (float)(currentChar.Advance) * fontSize / 66000.0f; //Advance by space character width
					currentIndex++; //Increment index
					continue; //Skip remaining part of loop
				}
				else if (*c == '§') {

					char nextChar = text.at(currentIndex + 1); //Gets char after the § symbol
					try {
						uint8_t colorCode = se_application.getTextColor(nextChar); //Get color code 
						currentColor = colorCode;
					}
					catch (const std::out_of_range& e) {
						se_error("Color code does not exist!");
					}

					c += 1; //Increment iterator by one to avoid §x, since loop will automatically increment by one as well
					currentIndex += 2; //Increment index by two to avoid §x, since we will skip the section of the loop where index is incremented once

					continue; //Skip remaining  part of loop
				}
				else {
					currentColor = 0xFF; //Default color is white
				}


				//Fill to next line logic ---------------------------
				if (currentLineWidth + totalCharSpace > textBox.getSize().x) { //Checks if line width is greater than text box width

					charY -= lineSpacing * fontSize / 156.25f; //Multiply by 256 to get pixel value
					charX = position.x;

					//currentIndex = 0; //Reset current Index
					currentLineWidth = 0;
				}
				//---------------------------------------------------

				setCharPosition(vec2(charX + (fontSize * (float)currentChar.Bearing.x / 50000.0f),
					charY + (fontSize * (float)currentChar.Bearing.y / 50000.0f)),
					charAmt);
				setCharTexture(currentChar.TextureID, charAmt);

				charAmt++; //Increment instance count
				currentLineWidth += totalCharSpace; //Add space taken up by character to line width
				currentIndex++;

				charX += resolutionScalingFactor * (float)(currentChar.Advance) * fontSize / 50000.0f;

			}
			setGLBufferDataNoScale(textBoxOffset + bufferStartIndex + totalCharAmt, charAmt); //Copies the temp buffers to the GL buffers
			totalCharAmt += charAmt;
			charAmt = 0;
		}
		else {
			for (c = section.begin(); c != section.end(); c++) {

				Shmingo::Character currentChar = se_application.getCharacterFontInfo(*c); //Get glyph info about current character
				std::size_t index = std::distance(section.begin(), c);
				float totalCharSpace = (float)(currentChar.Advance) * fontSize / 10000.0f; //Total space taken up by a character

				//Check for special characters
				if (*c == '\n') { //New line
					charY -= lineSpacing * 256.0f * fontSize / 10000.0f; //Multiply by 256 to get pixel value
					charX = position.x; //Reset x position for new line
					continue; //Skip remaining part of loop
				}
				else if (*c == ' ') {//Space character, avoid logic
					charX += (float)(currentChar.Advance) * fontSize / 66000.0f; //Advance by space character width
					continue; //Skip remaining part of loop
				}

				//Fill to next line logic ---------------------------
				if (currentLineWidth + totalCharSpace > textBox.getSize().x) { //Checks if line width is greater than text box width

					charY -= lineSpacing * fontSize / 156.25f; //Multiply by 256 to get pixel value
					charX = position.x;

					//currentIndex = 0; //Reset current Index
					currentLineWidth = 0;
				}
				//---------------------------------------------------

				setCharPosition(vec2(charX + (fontSize * (float)currentChar.Bearing.x / 50000.0f),
					charY + (fontSize * (float)currentChar.Bearing.y / 50000.0f)),
					charAmt);


				charAmt++; //Increment instance count
				currentLineWidth += totalCharSpace; //Add space taken up by character to line width

				charX += resolutionScalingFactor * (float)(currentChar.Advance) * fontSize / 50000.0f;

			}
			setGLBufferDataPositionsOnly(textBoxOffset + bufferStartIndex + totalCharAmt, charAmt); //Copies the temp buffers to the GL buffers
			totalCharAmt += charAmt;
			charAmt = 0;
		}
	}
}




void TextVertexArray::setGLBufferData(GLuint textBoxOffset, GLuint charAmt) {

	bindVao();

	glBindBuffer(GL_ARRAY_BUFFER, charTextureVboID);
	glBufferSubData(GL_ARRAY_BUFFER, textBoxOffset, charAmt, textureTempBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, colorVboID);
	glBufferSubData(GL_ARRAY_BUFFER, textBoxOffset, charAmt, colorTempBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, scaleVboID);
	glBufferSubData(GL_ARRAY_BUFFER, textBoxOffset, charAmt, scaleTempBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, positionsVboID);
	glBufferSubData(GL_ARRAY_BUFFER, textBoxOffset * 2 * sizeof(float), 2 * charAmt * sizeof(float), positionsTempBuffer);
}

void TextVertexArray::setGLBufferDataNoScale(GLuint textBoxOffset, GLuint charAmt) {

	bindVao();

	glBindBuffer(GL_ARRAY_BUFFER, charTextureVboID);
	glBufferSubData(GL_ARRAY_BUFFER, textBoxOffset, charAmt, textureTempBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, colorVboID);
	glBufferSubData(GL_ARRAY_BUFFER, textBoxOffset, charAmt, colorTempBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, positionsVboID);
	glBufferSubData(GL_ARRAY_BUFFER, textBoxOffset * 2 * sizeof(float), 2 * charAmt * sizeof(float), positionsTempBuffer);
}

void TextVertexArray::setGLBufferDataPositionsOnly(GLuint textBoxOffset, GLuint charAmt) {
	glBindBuffer(GL_ARRAY_BUFFER, positionsVboID);
	glBufferSubData(GL_ARRAY_BUFFER, textBoxOffset * 2 * sizeof(float), 2 * charAmt * sizeof(float), positionsTempBuffer);
}

GLuint TextVertexArray::loadTempBuffers(TextBox* textBox) {

	float resolutionScalingFactor = ((float)se_application.getWindow()->getHeight()) / ((float)se_application.getWindow()->getWidth());

	int currentIndex = 0; //Index of current character in text, including spaces and symbols
	float currentLineWidth = 0; //Current width of line

	float fontSize = (float)textBox->getFontSize();
	float lineSpacing = (float)textBox->getLineSpacing();
	vec2 position = textBox->getPosition();

	GLuint charAmt = 0;
	GLuint totalCharOffset = textBox->getCharacterOffsetInVao(); //Index of current character in buffer

	std::string::iterator c;

	std::string text = textBox->getText();

	//Set position of text box to just be x
	float charX = position.x;
	//Correct y position to be in correct position after character bearing, also change sign in order to clamp y between 0 and 1 instead of 0 and -1
	float charY = (-1.0f * position.y) - lineSpacing * fontSize / 100.0f;

	uint8_t currentColor = 0xFF; //Default color is white

	for (c = text.begin(); c != text.end(); c++) {

		Shmingo::Character currentChar = se_application.getCharacterFontInfo(*c); //Get glyph info about current character
		float totalCharSpace = (float)(currentChar.Advance) * fontSize / 10000.0f; //Total space taken up by a character

		setCharColor(currentColor, charAmt); //Defaults to white
		std::size_t index = std::distance(text.begin(), c);

		//Check for special characters
		if (*c == '\n') { //New line
			charY -= lineSpacing * 256.0f * fontSize / 10000.0f; //Multiply by 256 to get pixel value
			charX = position.x; //Reset x position for new line
			currentIndex++; //Increment index
			continue; //Skip remaining part of loop
		}
		else if (*c == ' ') {//Space character, avoid logic
			charX += (float)(currentChar.Advance) * fontSize / 66000.0f; //Advance by space character width
			currentIndex++; //Increment index
			continue; //Skip remaining part of loop
		}
		else if (*c == '§') {

			char nextChar = text.at(currentIndex + 1); //Gets char after the § symbol
			try {
				uint8_t colorCode = se_application.getTextColor(nextChar); //Get color code 
				currentColor = colorCode;
			}
			catch (const std::out_of_range& e) {
				se_error("Color code does not exist!");
			}

			c += 1; //Increment iterator by one to avoid §x, since loop will automatically increment by one as well
			currentIndex += 2; //Increment index by two to avoid §x, since we will skip the section of the loop where index is incremented once

			continue; //Skip remaining  part of loop
		}
		else {
			currentColor = 0xFF; //Default color is white
		}


		//Fill to next line logic ---------------------------
		if (currentLineWidth + totalCharSpace > textBox->getSize().x) { //Checks if line width is greater than text box width

			charY -= lineSpacing * fontSize / 156.25f; //Multiply by 256 to get pixel value
			charX = position.x;

			//currentIndex = 0; //Reset current Index
			currentLineWidth = 0;
		}
		//---------------------------------------------------

		setCharPosition(vec2(charX + (fontSize * (float)currentChar.Bearing.x / 50000.0f),
			charY + (fontSize * (float)currentChar.Bearing.y / 50000.0f)),
			charAmt);
		setCharTexture(currentChar.TextureID, charAmt);
		setCharSize((uint8_t)fontSize, charAmt);

		currentLineWidth += totalCharSpace; //Add space taken up by character to line width
		currentIndex++; //Increment index
		totalCharOffset++;

		charX += resolutionScalingFactor * (float)(currentChar.Advance) * fontSize / 50000.0f;
		//Vertex object wide
		charAmt++; //Increment instance count
		//End of char loop
	}

	return charAmt; //Return amount of characters submitted
}