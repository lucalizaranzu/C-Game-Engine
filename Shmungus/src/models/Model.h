#pragma once

#include <sepch.h>
#include <ShmingoCore.h>
#include "Vertex.h"
#include "Texture.h"

//Serves as just a wrapper around a std::array containing vertices, and eventually indices as well. 
//Future edits to this include animations and transforms.
//Look into submitting SSBOs to the gpu instead of animating and transforming actual vertex data


class Model{

public:

	Model(EntityVertex* vertices, GLuint vertexCount, int* indices, GLuint indexCount, Texture texture);
	~Model();
	inline GLuint getVertexCount() { return vertexCount; };
	inline EntityVertex* getVertexData() {return vertexData; };
	inline GLuint getVerticesByteSize() { return (vertexCount * sizeof(EntityVertex)); };

	inline GLuint getIndexCount() { return indexCount; };
	inline int* getIndexData() { return indexData; };
	inline GLuint getIndicesByteSize() { return (indexCount * sizeof(int)); };

private:

	//Vertex data
	GLuint vertexCount;
	EntityVertex* vertexData; //Needs to be a dynamically allocated pointer instead of a modern array, we can't specify size in advance

	//Index data
	GLuint indexCount;
	int* indexData;

	//Texture data
	Texture texture;
};

