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
	TextVertexArray();
	~TextVertexArray();


	//Vertex data functions ---------------------------------------------------

	void submitStaticText(TextBox textBox);
	void submitDynamicText(DynamicTextBox textBox);


	void updateDynamicTextBox(DynamicTextBox textBox);


	void removeTextBox(GLuint offset); //TODO change this to take in correct params



	//Getters ------------------------------------------------------------------
	inline GLuint getVaoID() { return vaoID; };
	inline GLuint getVertexCount() { return vertexCount; };
	inline GLuint getIndexCount() { return indexCount; };
	inline GLuint getInstanceAmount() { return instanceAmount; };

	inline GLuint getAttribAmount() { return attribAmount; };

protected:

	GLuint attribAmount = 0;

	GLuint vaoID = 0; //VAO ID
	GLuint eboID = 0; //Index Buffer ID

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

	//Utility functions --------------------------------------------------------

	inline void bindVao() {
		glBindVertexArray(vaoID);
	}
	inline void bindIndicesVbo() {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);
	}
	inline void unbindBuffer() {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

};