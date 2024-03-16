#pragma once

#include <sepch.h>
#include <ShmingoCore.h>
#include "Vertex.h"
#include "Texture.h"

//Serves as just a wrapper around a std::array containing vertices, and eventually indices as well. 
//Future edits to this include animations and transforms.
//Look into submitting SSBOs to the gpu instead of animating and transforming actual vertex data

//Has dynamically allocated members, so we need to employ the rule of three to be able to copy and delete these objects
class Model{

public:

	//Explicit to avoid implicit casting for a copy constructor
	explicit Model(float* positions, float* texCoords, Texture2D texture, GLuint vertexCount, int* indices, GLuint indexCount); 

	//Copy constructor
	Model(const Model& other);
	~Model();

	void instantiate();

	Model& operator=(const Model& other);


	inline GLuint getVertexCount() { return vertexCount; };


	inline float* getPositionData() { return positionData; };
	inline float* getTextureCoords() { return textureCoords; };

	inline GLuint getVerticesByteSize() { return (vertexCount * sizeof(EntityVertex)); };

	inline GLuint getIndexCount() { return indexCount; };
	inline int* getIndexData() { return indexData; };
	inline GLuint getIndicesByteSize() { return (indexCount * sizeof(int)); };

	inline Texture2D getTexture() { return texture; };

	std::unique_ptr<float[]> createTexIDArray();

private:


	//Vertex data
	GLuint vertexCount;

	float* positionData;
	float* textureCoords;
	Texture2D texture;

	//Index data
	GLuint indexCount;
	int* indexData;
};

