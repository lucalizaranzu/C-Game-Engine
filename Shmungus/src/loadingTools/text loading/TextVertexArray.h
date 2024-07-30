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

	void submitStaticText(TextBox textBox);
	void submitDynamicText(DynamicTextBox textBox);


	void updateDynamicTextBox(DynamicTextBox textBox);


	void removeTextBox(GLuint offset); //TODO change this to take in correct params


	void recalculateSpacing(float oldWidth, float oldHeight, float newWidth, float newHeight); //Recalculates the spacing between characters when the screen is resized



	//Getters ------------------------------------------------------------------
	inline GLuint getVaoID() { return vaoID; };
	inline GLuint getVertexCount() { return vertexCount; };
	inline GLuint getIndexCount() { return indexCount; };
	inline GLuint getInstanceAmount() { return instanceAmount; };

	inline GLuint getAttribAmount() { return attribAmount; };

	inline std::string getFont() { return fontName; };

	//Utility functions --------------------------------------------------------

	void cleanUp();

protected:

	GLuint attribAmount = 0;

	GLuint vaoID = 0; //VAO ID

	GLuint vertexPositionsVboID = 0;
	GLuint texCoordsVboID = 0;

	GLuint charTextureVboID = 0;
	GLuint colorVboID = 0;
	GLuint positionsVboID = 0;
	GLuint scaleVboID = 0;

	std::vector<GLuint> perInstanceVboIDs; //Per instance uniform VBO IDs

	GLuint vertexCount = 0; //Amount of vertices
	GLuint instanceAmount = 0;
	GLuint indexCount = 0; //Amount of indices
	GLuint maxTextureIndex = 0;

	std::string fontName;

	//Utility functions --------------------------------------------------------

	inline void bindVao() {
		glBindVertexArray(vaoID);
	}

	inline void unbindBuffer() {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	//Subdata methods to set buffer data for character
	void setCharTexture(uint8_t textureID, GLuint charOffset);
	void setCharSize(uint8_t size, GLuint charOffset);
	void setCharColor(uint8_t colorCode, GLuint charOffset);
	void setCharPosition(vec2 position, GLuint charOffset);
};