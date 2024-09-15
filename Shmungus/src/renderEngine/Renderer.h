#pragma once

#include "DefaultShader.h"
#include "EntityVertexArray.h"
#include "TextVertexArray.h"
#include "instancedVertexArray.h"
#include "ChunkVertexArray.h"

namespace Shmingo {

	/// <summary>
	/// Renders an instanced vertex array using provided shader
	/// </summary>
	/// <param name="vertexArray">
	/// Instanced vertex array to render
	/// </param>
	/// <param name="shader">
	/// Pointer to shader program to use for rendering
	/// </param>
	void renderEntity(std::shared_ptr<EntityVertexArray> vertexArray, std::shared_ptr<ShaderProgram> shader);

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

	/// <summary>
	/// Renders a generic instanced vertex array using provided shader
	/// </summary>
	/// <param name="vertexArray"></param>
	/// <param name="shader"></param>
	void renderInstanced(std::shared_ptr<InstancedVertexArray> vertexArray, std::shared_ptr<ShaderProgram> shader);

	/// <summary>
	/// Renders terrain
	/// </summary>
	/// <param name="vertexArray">Terrain Vertex Array</param>
	/// <param name="shader"></param>
	void renderTerrain(std::shared_ptr<ChunkVertexArray> vertexArray, std::shared_ptr<ShaderProgram> shader);

}