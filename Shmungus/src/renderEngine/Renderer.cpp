#include <sepch.h>
#include "Renderer.h"


void render(std::shared_ptr<VertexArray> vertexArray, std::shared_ptr<ShaderProgram> shader) {

	shader->start(); //Start shader

	glBindVertexArray(vertexArray->getVaoID()); //Bind VAO
	enableAttribs(vertexArray->getAttribAmount()); //Enable attribute arrays

	clearOpenGLError();
	glDrawElements(GL_TRIANGLES, vertexArray->getIndexCount(), GL_UNSIGNED_INT, NULL);
	checkOpenGLError();

	disableAttribs(vertexArray->getAttribAmount()); //Disable attribute arrays
	glBindVertexArray(0); //Unbind VAO

	shader->stop(); //Stop shader
}

void enableAttribs(unsigned int attribAmount) {
	for (int i = 0; i < attribAmount; i++) {
		glEnableVertexAttribArray(i);
	}
}

void disableAttribs(unsigned int attribAmount) {
	for (int i = 0; i < attribAmount; i++) {
		glDisableVertexAttribArray(i);
	}
}