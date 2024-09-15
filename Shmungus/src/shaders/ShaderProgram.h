#pragma once

#include "ShmingoCore.h"
#include "UniformBuffer.h"


enum ShaderType {

	se_SHADER_NONE,
	se_DEFAULT_SHADER,
	se_ENTITY_SHADER,
	se_TEXT_SHADER,
	se_MENU_SHADER,
	se_TERRAIN_SHADER

};

enum ShaderProgramType {

	DEFAULT_SHADER

};

using String = std::string;

String get_file_contents(const char* filename);


class ShaderProgram {

	friend class ShaderTools;

private:

public:

	ShaderProgram(std::string vertexFile, std::string fragmentFile);

	GLuint getProgramID();

	void start();
	void stop();

	void compileErrors(GLuint shader, const char* type);

	void cleanUp();

	void link();


	void loadTextureMap(int slotAmount);
	void loadSampler(const char* uniformName, GLuint value); //Pretty much just for testingp

	//Sets block binding paramters for shader
	template<typename... Args>
	void bindUniformBlocks(Args&&... blocks) {
		auto bindFunc = [&](auto&& block) {
			GLuint blockIndex = glGetUniformBlockIndex(ID, se_uniformBuffer.getBlockInfo(block).name);
			if (blockIndex == GL_INVALID_INDEX) {
				std::cerr << "Could not get index of uniform block " << se_uniformBuffer.getBlockInfo(block).name << std::endl;
				return; // Skip binding this block
			}

			// Ensure unique binding points for each block
			GLuint bindingPoint = se_uniformBuffer.getBlockInfo(block).bindingPoint;

			glUniformBlockBinding(ID, blockIndex, bindingPoint);
			};

		(bindFunc(std::forward<Args>(blocks)), ...);
	}

	void bindUniformBlock(Shmingo::UniformBlock block);


protected:

	const char* vertexFile; //Setting these as local variables so we don't need to pass in a constructor
	const char* fragmentFile; //Setting these as local variables so we don't need to pass in a constructor

	virtual void getAllUniformLocations();

	virtual void bindAttributes();
	virtual void bindAttribute(GLuint attribLocation, const GLchar* attribName);

	int getUniformLocation(const GLchar* uniformName);

	void loadFloat(GLuint location, float f);
	void loadInt(GLuint location, int i);
	void loadBool(GLuint location, bool b);

	void loadVector2(GLuint location, vec2 vec);
	void loadVector3(GLuint location, vec3 vec);
	void loadMatrix3(GLuint location, const mat4 mat);
	void loadMatrix4(GLuint location, const mat4 mat);

	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint ID;

};
