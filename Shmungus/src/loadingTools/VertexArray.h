#pragma once

#include <ShmingoCore.h>
#include "Vertex.h"

class VertexArray {

public:

	VertexArray();
	~VertexArray();

	inline GLuint getVaoID() { return this->vaoID; };
	
	void bind();
	void unbind();
	void pushTestVertices();

	inline GLuint getVertexCount() { return vertexCount; };
	inline GLuint getIndexCount() { return indexCount; };
	inline unsigned int getAttribAmount() { return attribAmount; };

protected:

	GLuint vaoID = 0; //This is a default value which will be overriden in the base class constructor when glGenVertexArrays() is called.

	GLuint vertexCount = 0; //Amount of vertices
	GLuint indexCount = 0; //Amount of indices

	GLuint vertexVboID = 0; //Same idea, one VBO per VAO
	GLuint indexVboID = 0; //Same idea, one VBO per VAO


	int verticesEndIndex = 0; //Index to write vertices into
	int indicesEndIndex = 0; //Index to write indices into, I believe fewer instructions than doing the math

	void bindVerticesVbo();
	void bindIndicesVbo();

	unsigned int attribAmount = 0; //Defaults to 0, declare this in child constructors!
};

class DefaultVertexArray : public VertexArray {

	DefaultVertexArray();

	void pushVertexData(DefaultVertex* vertices, GLuint verticesAmt, int* indices, GLuint indicesAmt);

private:

	const static unsigned int floatsPerVertex = 8;

};

class PositionsOnlyVertexArray : public VertexArray {

	PositionsOnlyVertexArray();

	void pushVertexData(PositionsOnlyVertex* vertices, GLuint verticesAmt, int* indices, GLuint indicesAmt);

private:

	const static unsigned int floatsPerVertex = 3;

};

class TexturedQuadVertexArray : public VertexArray {

	TexturedQuadVertexArray();

	void pushVertexData(TexturedQuadVertex* vertices, GLuint verticesAmt);

private:

	const static unsigned int floatsPerVertex = 4;

};

class ColorQuadVertexArray : public VertexArray {

public:

	ColorQuadVertexArray();

	void pushVertexData(ColorQuadVertex* vertices, GLuint verticesAmt);

private:

	const static unsigned int floatsPerVertex = 6;

};