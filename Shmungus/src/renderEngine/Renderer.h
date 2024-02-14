#pragma once

#include "DefaultShader.h"
#include "VertexArray.h"


void render(std::shared_ptr<VertexArray> vertexArray, std::shared_ptr<ShaderProgram> shader);

//Automatically enables vertex attributes
void enableAttribs(unsigned int attribAmount);
//Automatically disables vertex attributes
void disableAttribs(unsigned int attribAmount);

