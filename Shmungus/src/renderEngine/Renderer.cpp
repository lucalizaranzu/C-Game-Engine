#include <sepch.h>

#include "Renderer.h"

#include "Texture.h"
#include "TextureTools.h"
#include "Layer.h"
#include "ShmingoApp.h"

void enableAttribs(size_t attribAmount) {
	for (GLuint i = 0; i < attribAmount; i++) {
		glEnableVertexAttribArray(i);
	}
}

void disableAttribs(size_t attribAmount) {
	for (size_t i = 0; i < attribAmount; i++) {
		glDisableVertexAttribArray(i);
	}
}


void Shmingo::renderEntity(std::shared_ptr<EntityVertexArray> vertexArray, std::shared_ptr<ShaderProgram> shader) {

	shader->start(); //Start shader

	vertexArray->bindTextures(); //Load textures into texture slots

	glBindVertexArray(vertexArray->getVaoID()); //Bind VAO
	enableAttribs(7); //Enable attribute arrays

	glDrawElementsInstanced(GL_TRIANGLES, vertexArray->getIndexCount(), GL_UNSIGNED_INT, 0, vertexArray->getInstanceAmount());

	disableAttribs(vertexArray->getAttribAmount()); //Disable attribute arrays
	glBindVertexArray(0); //Unbind VAO

	shader->stop(); //Stop shader

}

void Shmingo::renderText(std::shared_ptr<TextVertexArray> vertexArray, std::shared_ptr<ShaderProgram> shader){

	shader->start();

	Shmingo::bindFontTextureToShader(shader, vertexArray->getFont());

	glBindVertexArray(vertexArray->getVaoID()); //Bind VAO

	enableAttribs(6);
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 6, (GLsizei)vertexArray->getInstanceAmount());
	disableAttribs(vertexArray->getAttribAmount()); //Disable attribute arrays
	glBindVertexArray(0); //Unbind VAO

	shader->stop(); //Stop shader

}

void Shmingo::renderInstanced(std::shared_ptr<InstancedVertexArray> vertexArray, std::shared_ptr<ShaderProgram> shader){

	shader->start();

	vertexArray->bindVao(); //Bind VAO

	vertexArray->bindTextures(); //Load textures into texture slots

	enableAttribs(vertexArray->getAttribAmt()); //Enables attributes
	clearOpenGLError();
	glDrawElementsInstanced(GL_TRIANGLES, vertexArray->getIndexCount(), GL_UNSIGNED_INT, 0, vertexArray->getInstanceCount());
	checkOpenGLError();
	disableAttribs(vertexArray->getAttribAmt()); //Disables attributes

	glBindVertexArray(0); //Unbind VAO
	shader->stop(); //Stop shader
}


