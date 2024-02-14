#include "ShaderTools.h"

std::shared_ptr<ShaderProgram> createShaderProgram(ShaderType type, std::string vertexFile, std::string fragmentFile){

	//This is the path of the default shader directory, if sub directories are made, those edits need to be reflected in this method
	std::string shaderPath = "shaders/";
	//Concatenates path
	std::string vertexPath = shaderPath + vertexFile;
	std::string fragmentPath = shaderPath + fragmentFile;

	switch (type) {

	case se_DEFAULT_SHADER:
		return std::make_shared<DefaultShader>(vertexPath.c_str(), fragmentPath.c_str());

	}

	return std::make_shared<ShaderProgram>("ERROR", "ERROR");
}
