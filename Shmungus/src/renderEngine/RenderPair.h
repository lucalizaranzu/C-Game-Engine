#pragma once

#include <ShmingoCore.h>
#include "VertexArray.h"
#include "Renderer.h"
#include "ShaderProgram.h"
#include "TextVertexArray.h"

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

class InstancedRenderPair {

	friend class MasterRenderer;

public:

	InstancedRenderPair(std::shared_ptr<InstancedVertexArray> vertexArray, std::shared_ptr<ShaderProgram> shader) : vertexArray(vertexArray), shader(shader) {}

	GLuint getVertexArrayID() { return vertexArray->getVaoID(); };
	inline std::shared_ptr<ShaderProgram> getShader() { return shader; };

	inline void setShader(std::shared_ptr<ShaderProgram> newShader) { shader = newShader; };

private:

	std::shared_ptr<InstancedVertexArray> vertexArray;
	std::shared_ptr<ShaderProgram> shader;

};

class TextRenderPair {

	friend class MasterRenderer;

public:

	TextRenderPair(std::shared_ptr<TextVertexArray> vertexArray, std::shared_ptr<ShaderProgram> shader) : vertexArray(vertexArray), shader(shader) {}

	GLuint getVertexArrayID() { return vertexArray->getVaoID(); };
	inline std::shared_ptr<ShaderProgram> getShader() { return shader; };

	inline void setShader(std::shared_ptr<ShaderProgram> newShader) { shader = newShader; };

private:

	std::shared_ptr<TextVertexArray> vertexArray;
	std::shared_ptr<ShaderProgram> shader;

};