#include <sepch.h>

#include "Model.h"

Model::Model(EntityVertex* vertices, GLuint vertexCount, int* indices, GLuint indexCount, GLuint textureID) : vertexData(vertices), vertexCount(vertexCount), 
	indexData(indices), indexCount(indexCount), textureID(textureID) {


}

Model::~Model(){

}
