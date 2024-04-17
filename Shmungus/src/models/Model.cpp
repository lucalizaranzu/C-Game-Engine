#include <sepch.h>

#include "Model.h"

Model::Model(float* positions, float* texCoords, Texture2D texture, GLuint vertexCount, int* indices, GLuint indexCount) : positionData(positions),
	textureCoords(texCoords), texture(texture), vertexCount(vertexCount), indexData(indices), indexCount(indexCount) {

}

Model::Model(const Model& other) : positionData(new float(*other.positionData)), textureCoords(new float(*other.textureCoords)),
	indexData(new int(*other.indexData)), texture(other.texture), vertexCount(other.vertexCount), indexCount(other.indexCount){

	//Initializer list to copy all variables and keep dynamically allocated members intact
}

Model::~Model(){

	delete[] positionData;
	delete[] textureCoords;
	delete[] indexData;
}

Model& Model::operator=(const Model& other){


	if (this == &other) { //check to make sure we don't delete our own data
		return *this;
	}

	if (!this) {
		instantiate();
	}

	float* newPositions = new float[other.vertexCount * 3];
	float* newTexCoords = new float[other.vertexCount * 2];
	int* newIndices = new int[other.indexCount];

	std::memcpy(newPositions, other.positionData, other.vertexCount * 3 * sizeof(float));
	std::memcpy(newTexCoords, other.textureCoords, other.vertexCount * 2 * sizeof(float));
	std::memcpy(newIndices, other.indexData, other.indexCount * sizeof(int));

	delete[] positionData;
	delete[] textureCoords;
	delete[] indexData;

	positionData = newPositions;
	textureCoords = newTexCoords;
	indexData = newIndices;

	//Move other variables
	texture = other.texture;
	indexCount = other.indexCount;
	vertexCount = other.vertexCount;
	 
	return *this;
}

std::unique_ptr<float[]> Model::createTexIDArray(){

	std::unique_ptr<float[]> texIDArray(new float[vertexCount]);
	for(GLuint i = 0; i < vertexCount; i++){
		texIDArray.get()[i] = (float)texture.getVaoLocalSlot();
	}
	return texIDArray;
}

void Model::instantiate(){
	 
	this->positionData = new float;
	this->textureCoords = new float;
	this->indexData = new int;
	this->vertexCount = 0;
	this->indexCount = 0;
}
