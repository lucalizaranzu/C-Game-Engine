#include <sepch.h>
#include "UniformBuffer.h"

//Im hardcoding all the offsets of the uniforms in the buffer object to avoid having to look up values in a hashmap every frame, get over it
// 
//Size of mat4 = 64
//size of mat3 = 36
//Size of vec4 = 16
//Size of vec3 = 12
//Size of vec2 = 8
//Size of int / float = 4
//Make sure to format properly according to STD140 protocol!

//Incrementing based on size of data type
#define OFFSET_VIEWMATRIX 0
#define OFFSET_PROJECTIONMATRIX 64



//Indices of blocks are defined here. Add an incremented macro every time you want to add a new type of block
#define INDEX_MATRIXBLOCK 0


UniformBuffer::UniformBuffer() {

} 

void UniformBuffer::init() {

	//Add size of uniform type when adding a new uniform
	GLuint uboSize = (2 * sizeof(mat4));

	glGenBuffers(1, &uboID);
	glBindBuffer(GL_UNIFORM_BUFFER, uboID);
	glBufferData(GL_UNIFORM_BUFFER, uboSize, nullptr, GL_DYNAMIC_DRAW);

	/*glBindBufferRange specifies the size of a uniform block.
	* I have opted to use the method of having only one buffer corresponding to multiple blocks instead
	* of one buffer per block. Because its all so abstracted here I really don't see a reason to have to bind
	* and unbind different buffer objects and use different UBOs for everything
	* */

	glBindBufferRange(GL_UNIFORM_BUFFER, INDEX_MATRIXBLOCK, uboID, 0, (2 * sizeof(mat4))); //Range 1

}

void UniformBuffer::setProjectionMatrix(mat4 projectionMatrix){
	glBindBuffer(GL_UNIFORM_BUFFER,uboID);
	setUniformMat4(projectionMatrix, OFFSET_PROJECTIONMATRIX, INDEX_MATRIXBLOCK);
}

void UniformBuffer::setViewMatrix(mat4 viewMatrix){
	glBindBuffer(GL_UNIFORM_BUFFER, uboID);
	setUniformMat4(viewMatrix, OFFSET_VIEWMATRIX, INDEX_MATRIXBLOCK);
}


void UniformBuffer::setAsActive() {
	glBindBuffer(GL_UNIFORM_BUFFER, uboID);
}


void UniformBuffer::setUniform1f(float value, GLint offset, GLuint index) {
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(float), &value);
}


void UniformBuffer::setUniform1i(int value, GLint offset, GLuint index) {

	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(int), &value);
}


void UniformBuffer::setUniformVec2(vec2 value, GLint offset, GLuint index) {

	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(vec2), &value);
}


void UniformBuffer::setUniformVec3(vec3 value, GLint offset, GLuint index) {


	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(vec3), &value);
}


void UniformBuffer::setUniformVec4(vec4 value, GLint offset, GLuint index) {


	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(vec4), &value);
}


void UniformBuffer::setUniformMat3(mat3 value, GLint offset, GLuint index) {


	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(mat3), glm::value_ptr(value));
}


void UniformBuffer::setUniformMat4(mat4 value, GLint offset, GLuint index) {


	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(mat4), glm::value_ptr(value));
}