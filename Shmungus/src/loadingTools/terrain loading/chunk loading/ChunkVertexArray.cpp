#include <sepch.h>

#include "ChunkVertexArray.h"
#include "Matrices.h"



ChunkVertexArray::ChunkVertexArray(){

	float staticPositions[9] = {
		0.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,
		1.0f,0.0f,1.0f
	};


	glGenVertexArrays(1, &vaoID);

	glGenBuffers(1, &staticPositionsVboID);
	glGenBuffers(1, &positionsVboID);
	glGenBuffers(1, &IDVboID);

	bind();

	glBindBuffer(GL_ARRAY_BUFFER, staticPositionsVboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(staticPositions), &staticPositions, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // transform position
	glVertexAttribDivisor(0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, positionsVboID);
	glBufferData(GL_ARRAY_BUFFER, MAX_CHUNK_POLYGONS * 2, nullptr, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(1, 2, GL_UNSIGNED_BYTE, GL_FALSE, 2 * sizeof(uint8_t), (void*)0); // transform position
	glVertexAttribDivisor(1, 1);

	glBindBuffer(GL_ARRAY_BUFFER, IDVboID);
	glBufferData(GL_ARRAY_BUFFER, MAX_CHUNK_POLYGONS * 2, nullptr, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(2, 1, GL_UNSIGNED_SHORT, GL_FALSE, sizeof(short), (void*)0); // transform position
	glVertexAttribDivisor(2, 1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void ChunkVertexArray::init(){
	//TODO set uniforms after declaring a terrain shader in the master renderer
}


size_t ChunkVertexArray::submitTerrain(unsigned short* positionsData, unsigned short* ID, size_t polyAmount){
	bind();

	glBindBuffer(GL_ARRAY_BUFFER, positionsVboID);
	glBufferSubData(GL_ARRAY_BUFFER, 0, polyAmount * 2, positionsData);

	glBindBuffer(GL_ARRAY_BUFFER, IDVboID);
	glBufferSubData(GL_ARRAY_BUFFER, 0, polyAmount * 2, ID);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return polyAmount;
}




void ChunkVertexArray::setUniforms(){
	mat4* transformationsArray = new mat4[64]; //64 matrices for transforming triangles

	transformationsArray[0] = Shmingo::createTransformationMatrix(vec3(0, 0, 0), vec3(0, 0, 0), vec3(1, 1, 1));
	transformationsArray[1] = Shmingo::createTransformationMatrix(vec3(0, 0, 0), vec3(0, 0, glm::radians(180.0f)), vec3(1, 1, 1));
}