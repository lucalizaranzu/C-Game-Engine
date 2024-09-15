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

	std::shared_ptr<DefaultShader> entityShader = std::make_shared<DefaultShader>("entityVertex.glsl", "entityFragment.glsl");
	std::shared_ptr<DefaultShader> textShader = std::make_shared<DefaultShader>("text/textVertex.glsl", "text/textFragment.glsl");
	std::shared_ptr<DefaultShader> menuShader = std::make_shared<DefaultShader>("menu/menuVertex.glsl", "menu/menuFragment.glsl");
	std::shared_ptr<DefaultShader> terrainShader = std::make_shared<DefaultShader>("terrain/terrainVertex.glsl", "terrain/terrainFragment.glsl");

	entityShader->bindUniformBlocks(Shmingo::MATRIX_BLOCK, Shmingo::UTIL_BLOCK);
	textShader->bindUniformBlocks(Shmingo::MATRIX_BLOCK);
	menuShader->bindUniformBlocks(Shmingo::MATRIX_BLOCK);
	terrainShader->bindUniformBlocks(Shmingo::MATRIX_BLOCK);

	mapShader(se_ENTITY_SHADER, entityShader);
	mapShader(se_TEXT_SHADER, textShader);
	mapShader(se_MENU_SHADER, menuShader);
	mapShader(se_TERRAIN_SHADER, terrainShader);

	mapEntityShader(Shmingo::DefaultEntity, entityShader);

	declareShaderTextureMap(se_ENTITY_SHADER, 32);
}


void MasterRenderer::declareShaderTextureMap(ShaderType type, int textureSlotAmount){

	shaderMap.at(type)->loadTextureMap(textureSlotAmount);
}


void MasterRenderer::submitEntityVertexArray(std::shared_ptr<EntityVertexArray> vertexArray){

	EntityRenderPair pair = EntityRenderPair(vertexArray, getEntityShader(vertexArray->getEntityType()));
	entityRenderQueue.emplace_back(pair);
}

void MasterRenderer::submitInstancedVertexArray(std::shared_ptr<InstancedVertexArray> vertexArray, ShaderType type){
	InstancedRenderPair pair = InstancedRenderPair(vertexArray, shaderMap.at(type));
	instancedRenderQueue.emplace_back(pair);
}

void MasterRenderer::submitTerrainVertexArray(std::shared_ptr<ChunkVertexArray> vertexArray, ShaderType type){
	TerrainRenderPair pair = TerrainRenderPair(vertexArray, shaderMap.at(type));
	terrainRenderQueue.emplace_back(pair);
}


void MasterRenderer::submitTextVertexArray(std::shared_ptr<TextVertexArray> vertexArray, ShaderType type) {
	TextRenderPair pair = TextRenderPair(vertexArray, shaderMap.at(type));
	textRenderQueue.emplace_back(pair);
}

void MasterRenderer::submitTextVertexArray(std::shared_ptr<TextVertexArray> vertexArray, std::shared_ptr<ShaderProgram> shader){
	TextRenderPair pair = TextRenderPair(vertexArray, shader);
	textRenderQueue.emplace_back(pair);
}


void MasterRenderer::renderInstancedBatch(){
	for (InstancedRenderPair pair : instancedRenderQueue) {
		//Uses render method from renderer.h
		Shmingo::renderInstanced(pair.vertexArray, pair.shader);
	}
}

void MasterRenderer::renderEntityBatch(){
	for (EntityRenderPair pair : entityRenderQueue) {
		//Uses render method from renderer.h
		Shmingo::renderEntity(pair.vertexArray, pair.shader);
	}
}

void MasterRenderer::renderTextBatch() {
	for (TextRenderPair pair : textRenderQueue) {
		//Uses render method from renderer.h
		Shmingo::renderText(pair.vertexArray, pair.shader);
	}
}

void MasterRenderer::renderTerrainBatch(){
	for (TerrainRenderPair pair : terrainRenderQueue) {
		//Uses render method from renderer.h
		Shmingo::renderTerrain(pair.vertexArray, pair.shader);
	}
}

void MasterRenderer::clearBatches() {
	entityRenderQueue.clear();
	textRenderQueue.clear();
	instancedRenderQueue.clear();
	terrainRenderQueue.clear();
}

//Renders the queue then clears it for next frame
void MasterRenderer::update() {

	renderEntityBatch();
	renderTextBatch();
	renderInstancedBatch();
	renderTerrainBatch();

	clearBatches();
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

void MasterRenderer::mapInstancedShader(ShaderType type, std::shared_ptr<ShaderProgram> shader){
	instancedShaderMap.insert(std::make_pair(type, shader));

}

/// <summary>
/// Returns a vector of information about each vertex attribute for a specific entity type
/// </summary>
/// <param name="type"></param>
/// <returns></returns>

/// <summary>
/// Returns a reference to the stored vector of information about each vertex attribute for a specific entity type
/// </summary>
/// <param name="type"></param>
/// <returns></returns>

std::vector<Shmingo::EntitySpecificInstanceDataInfo> MasterRenderer::getEntitySpecificInstanceAttributeInfo(Shmingo::EntityType type) {
	return entityUniformMap.at(type);
}

std::vector<Shmingo::EntitySpecificInstanceDataInfo>& MasterRenderer::getEntitySpecificInstanceAttributeInfoRef(Shmingo::EntityType type) {
	return entityUniformMap.at(type);
}

/// <summary>
/// Returns the amount of entity specific attributes for an entity type, major is amount of actual attributes, total is the amount of attributes including excess attribute lists corresponding to the same attribute
/// </summary>
/// <param name="type">Entity type</param>
/// <returns></returns>

//Gets amount of uniforms for a specific entity type
GLuint MasterRenderer::getEntitySpecificMajorInstanceAttribAmount(Shmingo::EntityType type) {
		return entitySpecificInstanceAttribAmountMap.at(type).front();
}
GLuint MasterRenderer::getEntitySpecificTotalInstanceAttribAmount(Shmingo::EntityType type) {
		return entitySpecificInstanceAttribAmountMap.at(type).back();
}

/// <summary>
/// Declares the vertex attributes for a specific entity type
/// </summary>
/// <param name="type"></param>
/// <param name="attributeInfo">
/// Vector of information about each vertex attribute for the entity type in a vector
/// </param>

void MasterRenderer::declareEntityInstanceAttributes(Shmingo::EntityType type, std::vector<Shmingo::EntitySpecificInstanceDataInfo> uniformInfo) {
	entityUniformMap.insert(std::make_pair(type, uniformInfo));
}

void MasterRenderer::declareEntitySpecificInstanceAttribAmount(Shmingo::EntityType type, GLuint majorAmount, GLuint totalAmount) {
	std::list<GLuint> list = { majorAmount, totalAmount };
	entitySpecificInstanceAttribAmountMap.insert(std::make_pair(type, list));
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

std::shared_ptr<ShaderProgram> MasterRenderer::getShader(ShaderType type){
	return shaderMap.at(type);
}
