#pragma once

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ShmingoCore.h"
#include "UniformBuffer.h"

enum ShaderType {

	se_SHADER_NONE,
	se_DEFAULT_SHADER

};

using String = std::string;

String get_file_contents(const char* filename);


class ShaderProgram {

	friend class ShaderTools;

private:

public:

	ShaderProgram(const char* vertexFile, const char* fragmentFile);

	GLuint getProgramID();

	void start();
	void stop();

	void compileErrors(GLuint shader, const char* type);

	void cleanUp();


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
