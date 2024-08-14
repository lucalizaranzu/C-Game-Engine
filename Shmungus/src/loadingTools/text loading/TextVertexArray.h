#pragma once

#pragma once

#include <ShmingoCore.h>
#include <sepch.h>

#include "TextBox.h"

//This class is for when we want to use instancing instead of batching, mostly for entities
//Templated to entity type for optimizations

class TextVertexArray {

public:

	//Constructor, sets up all necessary data
	TextVertexArray(std::string fontName);
	~TextVertexArray();


	//Vertex data functions ---------------------------------------------------

	void submitStaticText(TextBox& textBox);
	void submitDynamicText(DynamicTextBox& textBox);


	void updateDynamicTextBox(DynamicTextBox& textBox);


	void removeTextBox(GLuint offset, size_t textboxSize);


	void recalculateSpacing(float oldWidth, float oldHeight, float newWidth, float newHeight); //Recalculates the spacing between characters when the screen is resized



	//Getters ------------------------------------------------------------------
	inline GLuint getVaoID() { return vaoID; };
	inline size_t getVertexCount() { return vertexCount; };
	inline size_t getIndexCount() { return indexCount; };
	inline size_t getInstanceAmount() { return instanceAmount; };

	inline size_t getAttribAmount() { return attribAmount; };

	inline std::string getFont() { return fontName; };

	//Utility functions --------------------------------------------------------

	void cleanUp();

protected:

	size_t attribAmount = 0;

	GLuint vaoID = 0; //VAO ID

	GLuint vertexPositionsVboID = 0;
	GLuint texCoordsVboID = 0;

	GLuint charTextureVboID = 0;
	GLuint colorVboID = 0;
	GLuint positionsVboID = 0;
	GLuint scaleVboID = 0;

	GLuint charDataVboID = 0;

	std::vector<GLuint> perInstanceVboIDs; //Per instance uniform VBO IDs

	GLuint vertexCount = 0; //Amount of vertices
	size_t instanceAmount = 0;
	size_t staticTextBoxEmplaceOffset = 0; //Offset for emplacing static text boxes
	size_t indexCount = 0; //Amount of indices
	size_t maxTextureIndex = 0;

	std::string fontName;

	//Utility functions --------------------------------------------------------

	inline void bindVao() {
		glBindVertexArray(vaoID);
	}

	inline void unbindBuffer() {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	//Populates gl buffers with data from temp buffers
	void setGLBufferData(size_t textBoxOffset, size_t charAmt);
	void setGLBufferDataPositionsOnly(size_t textBoxOffset, size_t charAmt);

	//Temp buffers -------------------------------------------------------------

	float* positionsTempBuffer = new float[1000];
	Shmingo::GlyphData* charDataTempBuffer = new Shmingo::GlyphData[3000];

	//Populate temp buffers with text box data, returns amount of glyphs in the text box
	void uploadTextBox(TextBox* textBox);
	void uploadDynamicTextBox(DynamicTextBox* textBox);

	size_t uploadTextToTempBuffers(std::string text, size_t offsetInBuffer, vec2& pointerPosition, GLuint fontSize, GLuint lineSpacing, Shmingo::TextAlignment textAlignment, vec2 boundingBox, vec2 startingPosition, uint8_t defaultColor);

	void allocateSpaceForTextBox(TextBox* textBox);

	//Subdata methods to set buffer data for character
	
	void setCharData(Shmingo::GlyphData data, size_t offset);
	void setCharPosition(vec2 position, size_t charOffset);

	//Marks a character to be skipped in the shaders, used for padding for dynamic text
	void markCharForSkip(size_t offset);
	void markRangeForSkip(size_t bufferOffset, size_t skipAmt);

	//Reuploads dynamic text to the VAO
	void reuploadDynamicTextBox(DynamicTextBox& textBox);

	//Returns true if the character is to be used as a color code
	void uploadCharacterToTempBuffers(char c, uint8_t colorCode, size_t offsetInBuffer, vec2& pointerPosition, GLuint fontSize, GLuint lineSpacing, vec2 boundingBox, vec2 startingPosition);

	//Shifts the buffers to the right by shiftAmt
	void shiftBuffersRight(size_t offset, size_t shiftAmt);
	void shiftBuffersLeft(size_t offset, size_t shiftAmt);



	//Debug --------------------------------------------------------------------
	void printCharDataBuffer();

};