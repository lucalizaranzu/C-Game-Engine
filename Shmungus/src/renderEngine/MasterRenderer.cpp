#include <sepch.h>

#include "MasterRenderer.h"

UniformBuffer UniformBuffer::instance;

/*
* This is where all shaders will be declared with renderer pairs.
* You can specify the shader type enum, and the renderer and shaders that will go along with it.
* Use the new keyword to create a new pointer to a renderer and shader, make sure to specify shader paths.
* Notice how you can have multiple instances of the same type of shader with different file paths, this is intentional
* so that you don't have to make a new shaderprogram instance every time you want to use a new shader file
*/

void MasterRenderer::init() {

	se_uniformBuffer.init();

	declareRenderPair(se_DEFAULT_SHADER, createShaderProgram(DEFAULT_SHADER,"vertex.glsl","fragment.glsl"));

	declareRenderPair(se_ENTITY_SHADER, createShaderProgram(DEFAULT_SHADER, "entityVertex.glsl", "entityFragment.glsl"));
	declareShaderTextureMap(se_ENTITY_SHADER, 32);
}

//Declare render pairs for internal shaders, if you make a new shader type, add to the enum following the convention in ShaderProgram.h, then declare it as a pair with your new shader object
void MasterRenderer::declareRenderPair(ShaderType type, std::shared_ptr<ShaderProgram> shader) {

	//Add to maps with shader type as key
	shaderMap.insert(std::make_pair(type, shader));
}

void MasterRenderer::declareShaderTextureMap(ShaderType type, int textureSlotAmount){

	shaderMap.at(type)->loadTextureMap(textureSlotAmount);
}

//Creates a render group using the renderer and shader from the maps, adds to render queue
void MasterRenderer::submitVertexArray(std::shared_ptr<VertexArray> vertexArray, ShaderType type){

	RenderPair pair = RenderPair(vertexArray, shaderMap.at(type));
	renderQueue.emplace_back(pair);
}

void MasterRenderer::submitVertexArray(std::shared_ptr<VertexArray> vertexArray, std::shared_ptr<ShaderProgram> shader){

	RenderPair pair = RenderPair(vertexArray, shader);
	renderQueue.emplace_back(pair);
}

//Render all objects submitted
void MasterRenderer::renderBatch() { 
	for (RenderPair pair : renderQueue) {
		//Uses render method from renderer.h
		render(pair.vertexArray, pair.shader);
	}
}

void MasterRenderer::clearBatch() {
	renderQueue.clear();
}

//Renders the queue then clears it for next frame
void MasterRenderer::update() {

	renderBatch();
	clearBatch();
}