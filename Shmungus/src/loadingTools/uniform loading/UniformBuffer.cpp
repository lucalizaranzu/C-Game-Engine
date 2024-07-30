#include <sepch.h>
#include "UniformBuffer.h"
#include <ShmingoApp.h>

//Im hardcoding all the offsets of the uniforms in the buffer object to avoid having to look up values in a hashmap every frame, get over it
// 
//Size of mat4 = 64
//size of mat3 = 36
//Size of vec4 = 16
//Size of vec3 = 12
//Size of vec2 = 8
//Size of int / float = 4
//Make sure to format properly according to STD140 protocol!

//Incrementing based on size of data type, TODO make this not terrible and abstract to a list or something
#define OFFSET_ORTHOGRAPHICMATRIX 128
#define OFFSET_VIEWMATRIX 64
#define OFFSET_PROJECTIONMATRIX 0

#define OFFSET_ELAPSEDTIME 128



//Indices of blocks are defined here. Add an incremented macro every time you want to add a new type of block
#define INDEX_MATRIXBLOCK 0
#define INDEX_UTILBLOCK 1

GLuint matrixBlockSize = 128;
GLuint utilBlockSize = 4;

UniformBuffer::UniformBuffer() {}



void UniformBuffer::init() {

	//Add size of uniform type when adding a new uniform
	GLuint uboSize = (64 * 8);

	glGenBuffers(1, &uboID);
	glBindBuffer(GL_UNIFORM_BUFFER, uboID);
	glBufferData(GL_UNIFORM_BUFFER, uboSize, nullptr, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	createUniformBlock(Shmingo::MATRIX_BLOCK, 196, "Matrices", 0); //Range 0
	createUniformBlock(Shmingo::UTIL_BLOCK, 16, "Util", 1); //Range 1

	bindUniformBlock(Shmingo::MATRIX_BLOCK);
	bindUniformBlock(Shmingo::UTIL_BLOCK);
}

void UniformBuffer::setProjectionMatrix(mat4 projectionMatrix){
	glBindBuffer(GL_UNIFORM_BUFFER,uboID);
	setUniformMat4(projectionMatrix, OFFSET_PROJECTIONMATRIX);
}


void UniformBuffer::setOrthoMatrix(mat4 orthoMatrix) {
	glBindBuffer(GL_UNIFORM_BUFFER, uboID);
	setUniformMat4(orthoMatrix, OFFSET_ORTHOGRAPHICMATRIX);
}

void UniformBuffer::setViewMatrix(mat4 viewMatrix){
	glBindBuffer(GL_UNIFORM_BUFFER, uboID);
	setUniformMat4(viewMatrix, OFFSET_VIEWMATRIX);
}

void UniformBuffer::setElapsedTime(float time) {
	glBindBuffer(GL_UNIFORM_BUFFER, uboID);
	setUniform1f(time, 256 + 12);
}





void UniformBuffer::setAsActive() {
	glBindBuffer(GL_UNIFORM_BUFFER, uboID);
}


void UniformBuffer::setUniform1f(float value, GLint offset) {
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(float), &value);
}


void UniformBuffer::setUniform1i(int value, GLint offset) {

	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(int), &value);
}


void UniformBuffer::setUniformVec2(vec2 value, GLint offset) {

	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(vec2), &value);
}


void UniformBuffer::setUniformVec3(vec3 value, GLint offset) {


	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(vec3), &value);
}


void UniformBuffer::setUniformVec4(vec4 value, GLint offset) {


	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(vec4), &value);
}


void UniformBuffer::setUniformMat3(mat3 value, GLint offset) {


	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(mat3), glm::value_ptr(value));
}


void UniformBuffer::setUniformMat4(mat4 value, GLint offset) {


	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(mat4), glm::value_ptr(value));
}


void UniformBuffer::createUniformBlock(Shmingo::UniformBlock block, GLuint size, const char* name, GLuint bindingPoint) {

	//If empty
	if (blockInfo.keys.size() == 0) {

		blockInfo.insert(block,Shmingo::UniformBlockInfo{ name, size, 0, 0 });
	}
	else {
		Shmingo::UniformBlockInfo currentLast = blockInfo.at(blockInfo.keys.back());
		int newOffset = currentLast.offset + currentLast.size;

		int uniformBlockAlignment = se_application.getMinimumUniformBlockOffset();

		if (newOffset % uniformBlockAlignment != 0) {
			newOffset = newOffset + (256 - (newOffset % uniformBlockAlignment));
		}

		blockInfo.insert(block, Shmingo::UniformBlockInfo{ name, size, (GLuint)newOffset, bindingPoint });
	}
}

void UniformBuffer::bindUniformBlock(Shmingo::UniformBlock block){
	glBindBuffer(GL_UNIFORM_BUFFER, uboID);
	Shmingo::UniformBlockInfo currentBlockInfo = blockInfo.at(block);

	glBindBufferRange(GL_UNIFORM_BUFFER, currentBlockInfo.bindingPoint, uboID, currentBlockInfo.offset, currentBlockInfo.size);
}
