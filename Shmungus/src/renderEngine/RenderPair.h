#pragma once

#include <ShmingoCore.h>
#include "VertexArray.h"
#include "Renderer.h"
#include "ShaderProgram.h"

//This object contains a vertrex array, a shader and a renderer, used to avoid the need to lookup a shader and renderer every time
class RenderPair {

	friend class MasterRenderer;

public:

	RenderPair(std::shared_ptr<VertexArray> vertexArray, std::shared_ptr<ShaderProgram> shader) : vertexArray(vertexArray), shader(shader) {};

	GLuint getVertexArrayID() { return vertexArray->getVaoID(); };
	inline std::shared_ptr<ShaderProgram> getShader() { return shader; };

	inline void setShader(std::shared_ptr<ShaderProgram> newShader) { shader = newShader; };

private:

	std::shared_ptr<VertexArray> vertexArray;
	std::shared_ptr<ShaderProgram> shader;

};

template<typename E> //The same as a default renderpair, except the vertexarray parameter is templated for use with a templated render method.
class InstancedRenderPair {

public:

	InstancedRenderPair(std::shared_ptr<InstancedVertexArray<E>> vertexArray, std::shared_ptr<ShaderProgram> shader) : vertexArray(vertexArray), shader(shader) {}

	GLuint getVertexArrayID() { return vertexArray->getVaoID(); };
	inline std::shared_ptr<ShaderProgram> getShader() { return shader; };

	inline void setShader(std::shared_ptr<ShaderProgram> newShader) { shader = newShader; };

private:

	std::shared_ptr<InstancedVertexArray<E>> vertexArray;
	std::shared_ptr<ShaderProgram> shader;

};