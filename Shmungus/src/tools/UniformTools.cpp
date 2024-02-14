#include "UniformTools.h"

//Need to keep track of the ID of the currently bound shader program, so it can be passed into glGetUniformLocation
GLuint boundShaderID = 0;


void setActiveShaderProgram(std::shared_ptr<ShaderProgram> shader) {
	shader->start();
	boundShaderID = shader->getProgramID();
}


//These functions are all implemented individually to follow OpenGL's method of doing it, likely to avoid overhead of templates when operating multiple times per shader per frame
void uploadUniform1f(float value, const char* uniformName){
	GLint location = glGetUniformLocation(boundShaderID, uniformName);
	glUniform1f(location, value);
}

void uploadUniform1i(int value, const char* uniformName){
	GLint location = glGetUniformLocation(boundShaderID, uniformName);
	glUniform1i(location, value);
}

void uploadUniformVec2(vec2 value, const char* uniformName){
	GLint location = glGetUniformLocation(boundShaderID, uniformName);
	glUniform2f(location, value.x,value.y);
}

void uploadUniformVec3(vec3 value, const char* uniformName){
	GLint location = glGetUniformLocation(boundShaderID, uniformName);
	glUniform3f(location, value.x, value.y, value.z);
}

void uploadUniformVec4(vec4 value, const char* uniformName){
	GLint location = glGetUniformLocation(boundShaderID, uniformName);
	glUniform4f(location, value.x, value.y, value.z, value.w);
}

void uploadUniformMat3(mat3 value, const char* uniformName){
	GLint location = glGetUniformLocation(boundShaderID, uniformName);
	glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void uploadUniformMat4(mat4 value, const char* uniformName){
	GLint location = glGetUniformLocation(boundShaderID, uniformName);
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}
