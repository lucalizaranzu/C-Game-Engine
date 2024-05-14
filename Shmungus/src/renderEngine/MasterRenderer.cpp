#include <sepch.h>

#include "MasterRenderer.h"
#include "ShmingoApp.h"

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
	se_application.declareEntiyType(Shmingo::DefaultEntity);

	std::shared_ptr<DefaultShader> entityShader = std::make_shared<DefaultShader>("entityVertex.glsl", "entityFragment.glsl");

	entityShader->bindUniformBlocks(Shmingo::MATRIX_BLOCK, Shmingo::UTIL_BLOCK);

	mapShader(se_ENTITY_SHADER, entityShader);

	mapEntityShader(Shmingo::DefaultEntity, entityShader);

	declareShaderTextureMap(se_ENTITY_SHADER, 32);
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

void MasterRenderer::submitInstancedVertexArray(std::shared_ptr<InstancedVertexArray> vertexArray){

	//TODO implement deferred rendering? For now just render directly using the render instanced method
}

//Render all objects submitted
void MasterRenderer::renderBatch() { 
	for (RenderPair pair : renderQueue) {
		//Uses render method from renderer.h
		Shmingo::render(pair.vertexArray, pair.shader);
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






//Utility functions -----------------------------------------------------------------------------------------------

//Declare render pairs for internal shaders, if you make a new shader type, add to the enum following the convention in ShaderProgram.h, then declare it as a pair with your new shader object
void MasterRenderer::mapShader(ShaderType type, std::shared_ptr<ShaderProgram> shader) {

	//Add to maps with shader type as key
	shaderMap.insert(std::make_pair(type, shader));
}

void MasterRenderer::mapEntityShader(Shmingo::EntityType type, std::shared_ptr<ShaderProgram> shader) {
	entityShaderMap.insert(std::make_pair(type, shader));
}

void MasterRenderer::mapInstancedShader(std::type_index entityType, GLuint textureSlotAmount, std::shared_ptr<ShaderProgram> shader) {
	instancedShaderMap.insert(std::make_pair(entityType, shader));
	shader->loadTextureMap(textureSlotAmount);
}

/// <summary>
/// Returns a vector of information about each vertex attribute for a specific entity type
/// </summary>
/// <param name="type"></param>
/// <returns></returns>
std::vector<Shmingo::EntitySpecificVertexDataInfo> MasterRenderer::getEntityVertexAttributeInfo(Shmingo::EntityType type){
	return entityVertexAttributeMap.at(type);
}

/// <summary>
/// Returns the amount of entity specific attributes for an entity type
/// </summary>
/// <param name="type"></param>
/// <returns></returns>
GLuint MasterRenderer::getEntitySpecificAttribAmount(Shmingo::EntityType type){
	return entitySpecificAttribAmountMap.at(type);
}

/// <summary>
/// Declares the vertex attributes for a specific entity type
/// </summary>
/// <param name="type"></param>
/// <param name="attributeInfo">
/// Vector of information about each vertex attribute for the entity type in a vector
/// </param>
void MasterRenderer::declareEntityVertexAttributes(Shmingo::EntityType type, std::vector<Shmingo::EntitySpecificVertexDataInfo> attributeInfo){
	entityVertexAttributeMap.insert(std::make_pair(type, attributeInfo));
}

void MasterRenderer::declareEntitySpecificAttribAmount(Shmingo::EntityType type, GLuint amount){
	entitySpecificAttribAmountMap.insert(std::make_pair(type, amount));
}

/// <summary>
/// Declares the model an entity type will use
/// </summary>
/// <param name="type"></param>
/// <param name="model"></param>
void MasterRenderer::declareEntityModel(Shmingo::EntityType type, std::shared_ptr<Model> model){
	entityModelMap.insert(std::make_pair(type, model));
}

std::shared_ptr<Model> MasterRenderer::getEntityModel(Shmingo::EntityType type){
	return entityModelMap.at(type);
}

std::shared_ptr<ShaderProgram> MasterRenderer::getEntityShader(Shmingo::EntityType type){
	return entityShaderMap.at(type);
}