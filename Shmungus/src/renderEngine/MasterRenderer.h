#pragma once

#include <ShmingoCore.h>
#include <typeindex>

#include "ShaderTools.h"
#include "RenderPair.h"
#include "Renderer.h"
#include "DefaultShader.h"
#include "UniformBuffer.h"


class MasterRenderer {

public:

	inline static MasterRenderer& get() { return instance; };

	//Meant for initializing the maps, all shaders and renderers should be added to the map in this function
	void init();
	//This function is how you render an object using shaders provided by the engine, custom shaders pass in a custom shader object in a shared pointer
	void submitVertexArray(std::shared_ptr<VertexArray> vertexArray, ShaderType type);
	//Meant for shaders added by modders, which do not have a corresponding ShaderType enum. Pass the object directly into the function
	void submitVertexArray(std::shared_ptr<VertexArray>, std::shared_ptr<ShaderProgram> shader);

	template<typename E>
	void submitInstancedVertexArray(std::shared_ptr<InstancedVertexArray<E>> vertexArray) {

		renderInstanced<E>(vertexArray, instancedShaderMap.at(typeid(E))); //Render with the shader that corresponds to the type of the instanced vertex array

	}

	void update();


	void renderBatch();
	void clearBatch();


private:

	//Used to set which shader program and renderer a particular shader enum corresponds to
	void mapShader(ShaderType type, std::shared_ptr<ShaderProgram> shader);

	//Needs to have type explicitly declared, also declares an amount of texture slots
	void mapInstancedShader(std::type_index entityType, GLuint textureSlotAmount, std::shared_ptr<ShaderProgram> shader);

	//Used to set sampler uniform
	void declareShaderTextureMap(ShaderType type, int textureSlotAmount);

	MasterRenderer() {};

	static MasterRenderer instance;

	std::vector<RenderPair> renderQueue; 

	std::map<ShaderType, std::shared_ptr<ShaderProgram>> shaderMap;
	std::map<std::type_index, std::shared_ptr<ShaderProgram>> instancedShaderMap;
};

