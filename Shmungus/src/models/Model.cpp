#include <sepch.h>

#include "Model.h"

Model::Model(EntityVertex* vertices, GLuint vertexCount, int* indices, GLuint indexCount, Texture texture) : vertexData(vertices), vertexCount(vertexCount), 
	indexData(indices), indexCount(indexCount), texture(texture) {


}

Model::~Model(){

}
