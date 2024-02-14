#include "DefaultShader.h"

//Define uniform variable locations

DefaultShader::DefaultShader(const char* vertexFile, const char* fragmentFile) : ShaderProgram(vertexFile, fragmentFile) {

	glUniformBlockBinding(ID, glGetUniformBlockIndex(ID, "MatrixData"), 0); //Sets uniform binding point 0 to matrices

	bindAttributes();
}

void DefaultShader::bindAttributes() {

	bindAttribute(0, "positions"); // Define location and name of VBO attribute here
	bindAttribute(1, "color"); // Define location and name of VBO attribute here

}

void DefaultShader::getAllUniformLocations() {
	//Assign values to uniform location variables here using getUniformLocation function, which calls glGetUniform functions to associate the uniform name with its value


}

//Eventually define functions here that can be called outside to call the load functions from the parent class