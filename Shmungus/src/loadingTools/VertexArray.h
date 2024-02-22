#pragma once

#include <ShmingoCore.h>
#include "Vertex.h"
#include "Entity.h"

class VertexArray {

public:

	VertexArray();
	~VertexArray();


	//Main functions
	void pushTestVertices();
	
	//Helper functions
	void bind();
	void unbind();

	//Getters for information about buffer itself
	inline GLuint getVaoID() { return this->vaoID; };
	inline GLuint getVertexCount() { return vertexCount; };
	inline GLuint getIndexCount() { return indexCount; };
	inline unsigned int getAttribAmount() { return attribAmount; };

protected:

	//Attributes of buffer itself ------------------------------------------------------------
	GLuint vaoID = 0; //This is a default value which will be overriden in the base class constructor when glGenVertexArrays() is called.

	GLuint vertexCount = 0; //Amount of vertices
	GLuint indexCount = 0; //Amount of indices

	GLuint vertexVboID = 0; //Same idea, one VBO per VAO
	GLuint indexVboID = 0; //Same idea, one VBO per VAO

	unsigned int attribAmount = 0; //Defaults to 0, declare this in child constructors!


	//Useful information for member functions to know --------------------------------------
	int verticesEndIndex = 0; //Index to push new vertices into
	int indicesEndIndex = 0; //Index to write indices into, I believe fewer instructions than doing the math


	//Bookkeeping ---------------------------------------------------------------------------

	


	//Small helper functions ---------------------------------------------------------------------------
	void bindVerticesVbo(); //Bind the vertices VBO
	void bindIndicesVbo(); //Bind the indices VBO

};


//Child class declarations ----------------------
//Basic vertex arrays - more or less just proof of concept

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


//Realistically used vertex arrays

class EntityVertexArray : public VertexArray {

public:

	typedef std::vector<std::shared_ptr<Entity>> entityVec;

	EntityVertexArray();

	void pushVertexData(std::shared_ptr<Entity> entity);
	void removeVertexData(std::shared_ptr<Entity> entity);

private:

	entityVec insertOrderVector;

	std::map<std::shared_ptr<Entity>, GLuint> vertexOffsetMap;
	std::map<std::shared_ptr<Entity>, GLuint> indexOffsetMap;

};