#include <sepch.h>

#include "TextVertexArray.h"
#include "ShmingoApp.h"
#include "DataStructures.h"


uint8_t* textureTempBuffer = new uint8_t[500];
uint8_t* colorTempBuffer = new uint8_t[500];
float* positionsTempBuffer = new float[1000];
uint8_t* scaleTempBuffer = new uint8_t[500];

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


void TextVertexArray::submitStaticText(TextBox textBox){

	float resolutionScalingFactor = ((float)se_application.getWindow()->getHeight()) / ((float)se_application.getWindow()->getWidth());

	textBox.setVaoCharOffset(instanceAmount); //Sets char offset to end of buffer - change if resetting text later

	int currentIndex = 0; //Index of current character in text, including spaces and symbols
	float currentLineWidth = 0; //Current width of line

	GLuint charAmt = 0;
	GLuint totalCharOffset = textBox.getCharacterOffsetInVao(); //Index of current character in buffer

	std::string::const_iterator c;
	std::string text = textBox.getText();

	//Set position of text box to just be x
	float charX = textBox.getPosition().x;
	//Correct y position to be in correct position after character bearing, also change sign in order to clamp y between 0 and 1 instead of 0 and -1
	float charY = (-1.0f * textBox.getPosition().y) - ((float)textBox.getLineSpacing() * (float)textBox.getFontSize() / 100.0f);

	uint8_t currentColor = 0xFF; //Default color is white

	for (c = text.begin(); c != text.end(); c++) {

		Shmingo::Character currentChar = se_application.getCharacterFontInfo(*c); //Get glyph info about current character
		float totalCharSpace = (float)(currentChar.Advance) * (float)textBox.getFontSize() / 10000.0f; //Total space taken up by a character

		setCharColor(currentColor, charAmt); //Defaults to white

		//Check for special characters
		if (*c == '\n') { //New line
			charY -= ((float)textBox.getLineSpacing() * 256.0f * (float)textBox.getFontSize() / 10000.0f); //Multiply by 256 to get pixel value
			charX = textBox.getPosition().x;
			continue;
		}
		else if (*c == ' ') {//Space character, avoid logic
			charX += (float)(currentChar.Advance) * (float)textBox.getFontSize() / 66000.0f;
			currentIndex++;
			continue;
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

			charY -= ((float)textBox.getLineSpacing() * (float)textBox.getFontSize() / 156.25f); //Multiply by 256 to get pixel value
			charX = textBox.getPosition().x;



			//currentIndex = 0; //Reset current Index
			currentLineWidth = 0;
		}
		//---------------------------------------------------

		setCharPosition(vec2(charX + ((float)textBox.getFontSize() * (float)currentChar.Bearing.x / 50000.0f), 
			charY + ((float)textBox.getFontSize() * (float)currentChar.Bearing.y / 50000.0f)), 
			charAmt);
		setCharTexture(currentChar.TextureID, charAmt);
		setCharSize((uint8_t)textBox.getFontSize(), charAmt);

		//se_log("Submitting character " << *c << " at final buffer position " << charAmt << ": " << charX << ", " << charY);


		currentLineWidth += totalCharSpace; //Add space taken up by character to line width
		currentIndex++; //Increment index
		totalCharOffset++;

		charX += resolutionScalingFactor * (float)(currentChar.Advance) * (float)textBox.getFontSize() / 50000.0f;
		//Vertex object wide
		charAmt++; //Increment instance count
		//End of char loop
	}


	bindVao();

	GLuint textBoxOffset = textBox.getCharacterOffsetInVao();

	se_log("Text box offset: " << textBoxOffset);

	glBindBuffer(GL_ARRAY_BUFFER, charTextureVboID);
	glBufferSubData(GL_ARRAY_BUFFER, textBoxOffset, charAmt, textureTempBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, colorVboID);
	glBufferSubData(GL_ARRAY_BUFFER, textBoxOffset, charAmt, colorTempBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, scaleVboID);
	glBufferSubData(GL_ARRAY_BUFFER, textBoxOffset, charAmt, scaleTempBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, positionsVboID);
	glBufferSubData(GL_ARRAY_BUFFER, textBoxOffset * 2 * sizeof(float), 2 * charAmt * sizeof(float), positionsTempBuffer);

	se_log("submitted " << text.size() << " characters");
	se_log(charAmt << " characters in buffer");

	instanceAmount += charAmt;
}

void TextVertexArray::recalculateSpacing(float oldWidth, float oldHeight, float newWidth, float newHeight){

	glBindVertexArray(vaoID);
	glBindBuffer(GL_ARRAY_BUFFER, positionsVboID);

	float* data = (float*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE); //maps buffer to memory

	GLint bufferSize;
	glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &bufferSize);

	se_log(instanceAmount << " characters in buffer");

	float oldAspectRatio = oldWidth / oldHeight;
	float newAspectRatio = newWidth / newHeight;

	for (GLuint i = 0; i < instanceAmount; ++i) {
		float copy = data[i*2];
		data[i * 2] *= (oldAspectRatio / newAspectRatio); //Divides all x components by aspect ratio
		//data[(i * 2) + 1] *= (newHeight / oldHeight); //Divides all y components by aspect ratio
		se_log("X value at index " << i << ": before: " << copy << ", after: " << data[i*2]);
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
