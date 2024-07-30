#pragma once

#include "DefaultShader.h"
#include "VertexArray.h"
#include "InstancedVertexArray.h"
#include "TextVertexArray.h"

namespace Shmingo {

	/// <summary>
	/// Renders a standard vertex array using the supplied shader
	/// </summary>
	/// <param name="vertexArray">
	/// Vertex array to render
	/// </param>
	/// <param name="shader">
	/// Shader program to use for rendering
	/// </param>
	void render(std::shared_ptr<VertexArray> vertexArray, std::shared_ptr<ShaderProgram> shader);

	/// <summary>
	/// Renders an instanced vertex array using provided shader
	/// </summary>
	/// <param name="vertexArray">
	/// Instanced vertex array to render
	/// </param>
	/// <param name="shader">
	/// Pointer to shader program to use for rendering
	/// </param>
	void renderInstanced(std::shared_ptr<InstancedVertexArray> vertexArray, std::shared_ptr<ShaderProgram> shader);

	/// <summary>
	/// Renders an instanced vertex array designed for text using provided shader
	/// </summary>
	/// <param name="vertexArray">
	/// Instanced vertex array to render
	/// </param>
	/// <param name="shader">
	/// Pointer to shader program to use for rendering
	/// </param>
	void renderText(std::shared_ptr<TextVertexArray> vertexArray, std::shared_ptr<ShaderProgram> shader);

}