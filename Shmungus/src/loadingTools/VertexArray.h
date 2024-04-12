#pragma once

#include <ShmingoCore.h>

#include "Texture.h"
#include "Vertex.h"
#include "Entity.h"


class VertexArray {

public:

	VertexArray();
	~VertexArray();


	//Main functions
	void declareTextureSlot(Texture2D, GLuint slot);
	void bindTextures();
	
	//Helper functions
	void bind();
	void unbind();

	//Getters for information about buffer itself
	inline GLuint getVaoID() { return this->vaoID; };
	inline GLuint getVertexCount() { return vertexCount; };
	inline GLuint getIndexCount() { return indexCount; };
	inline unsigned int getAttribAmount() { return attribAmount; };

	inline Texture2D getTexture(GLuint slot) { return textureList[slot]; };

	//Debugging functions

protected:

	//Attributes of buffer itself ------------------------------------------------------------
	GLuint vaoID = 0; //This is a default value which will be overriden in the base class constructor when glGenVertexArrays() is called.

	GLuint vertexCount = 0; //Amount of vertices
	GLuint indexCount = 0; //Amount of indices

	GLuint indexVboID = 0; //Same idea, one VBO per VAO

	unsigned int attribAmount = 0; //Defaults to 0, declare this in child constructors!


	//Useful information for member functions to know --------------------------------------
	int verticesEndIndex = 0; //Index to push new vertices into
	int indicesEndIndex = 0; //Index to write indices into, I believe fewer instructions than doing the math


	//Bookkeeping ---------------------------------------------------------------------------

	//Array of texture IDs to be bound before rendering, index corresponds to slot
	std::vector<Texture2D> textureList;
	GLuint maxTextureIndex = 0;


	//Small helper functions ---------------------------------------------------------------------------
	void bindIndicesVbo(); //Bind the indices VBO

};


class InterleavedVertexArray : public VertexArray {

public:

	InterleavedVertexArray();

protected:


	GLuint vertexVboID = 0; //Interleaved vertex arrays have one VBO per VAO
	void bindVerticesVbo(); //Bind the vertices VBO

};

//Child class declarations ----------------------
//Basic vertex arrays - more or less just proof of concept

class DefaultVertexArray : public InterleavedVertexArray {

	DefaultVertexArray();

	void pushVertexData(DefaultVertex* vertices, GLuint verticesAmt, int* indices, GLuint indicesAmt);

private:

	const static unsigned int floatsPerVertex = 8;

};


class PositionsOnlyVertexArray : public InterleavedVertexArray {

	PositionsOnlyVertexArray();

	void pushVertexData(PositionsOnlyVertex* vertices, GLuint verticesAmt, int* indices, GLuint indicesAmt);

private:

	const static unsigned int floatsPerVertex = 3;

};


class TexturedQuadVertexArray : public InterleavedVertexArray {

	TexturedQuadVertexArray();

	void pushVertexData(TexturedQuadVertex* vertices, GLuint verticesAmt);

private:

	const static unsigned int floatsPerVertex = 4;

};


class ColorQuadVertexArray : public InterleavedVertexArray {

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

	GLuint positionsVboID = 0;
	GLuint texCoordsVboID = 0;
	GLuint textureIDVboID = 0;

	entityVec insertOrderVector;

	std::map<std::shared_ptr<Entity>, GLuint> vertexOffsetMap;
	std::map<std::shared_ptr<Entity>, GLuint> indexOffsetMap;

};