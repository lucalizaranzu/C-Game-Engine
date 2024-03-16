#pragma once

#include "DefaultShader.h"
#include "VertexArray.h"
#include "InstancedVertexArray.h"

#ifndef RENDERER_H
#define RENDERER_H

void render(std::shared_ptr<VertexArray> vertexArray, std::shared_ptr<ShaderProgram> shader);
//Automatically enables vertex attributes
void enableAttribs(GLuint attribAmount);
//Automatically disables vertex attributes
void disableAttribs(GLuint attribAmount);

//Templated because of the different templated vertex arrays
template<typename E>
void renderInstanced(std::shared_ptr<InstancedVertexArray<E>> vertexArray, std::shared_ptr<ShaderProgram> shader) {

	shader->start(); //Start shader

	vertexArray->bindTextures(); //Load textures into texture slots

	glBindVertexArray(vertexArray->getVaoID()); //Bind VAO
	enableAttribs(vertexArray->getAttribAmount()); //Enable attribute arrays

	clearOpenGLError();
	glDrawElementsInstanced(GL_TRIANGLES, vertexArray->getIndexCount(), GL_UNSIGNED_INT, 0, vertexArray->getInstanceAmount());

	checkOpenGLError();

	disableAttribs(vertexArray->getAttribAmount()); //Disable attribute arrays
	glBindVertexArray(0); //Unbind VAO

	shader->stop(); //Stop shader

}

#endif