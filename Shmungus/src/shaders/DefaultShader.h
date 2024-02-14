#pragma once

#include <ShaderProgram.h>

class DefaultShader : public ShaderProgram{



public:

	DefaultShader(const char* vertexFile, const char* fragmentFile);

private:

	void bindAttributes() override; //Override parent function
	void getAllUniformLocations() override;

protected:



};

