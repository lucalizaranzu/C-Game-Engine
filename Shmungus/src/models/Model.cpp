#include <sepch.h>

#include "Model.h"

Model::Model(EntityVertex* vertices, GLuint vertexCount, int* indices, GLuint indexCount) : vertexData(vertices), vertexCount(vertexCount), 
	indexData(indices), indexCount(indexCount) {


}

Model::~Model(){

	delete vertexData;
	delete indexData;

}
