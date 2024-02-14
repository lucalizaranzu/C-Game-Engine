#pragma once

#include <ShmingoCore.h>
#include "ShaderProgram.h"
#include "DefaultShader.h"

//Will add other tools eventually
std::shared_ptr<ShaderProgram> createShaderProgram(ShaderType type, std::string vertexFile, std::string fragmentFile);