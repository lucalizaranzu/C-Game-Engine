#pragma once

#include <ShmingoCore.h>
#include "ShaderProgram.h"

//Macros to follow the style of other functions designed to be used in higher level implementations
#define se_setActiveShader setActiveShaderProgram
#define se_uploadUniform1i uploadUniform1i
#define se_uploadUniform1f uploadUniform1f
#define se_uploadUniformVec2 uploadUniformVec2
#define se_uploadUniformVec3 uploadUniformVec3
#define se_uploadUniformVec4 uploadUniformVec4
#define se_uploadMat3 uploadUniformMat3
#define se_uploadMat4 uploadUniformMat4



//Sets the active shader to specified program, always call this before uploading uniforms to set which shader will be using them
void setActiveShaderProgram(std::shared_ptr<ShaderProgram> shader);

//Helper functions for uploading uniforms to custom shaders not part of any officially implemented UBOs

//Upload 1 float
void uploadUniform1f(float value, const char* uniformName);
//Upload 1 int
void uploadUniform1i(int value, const char* uniformName);
//Upload vector of 2 floats
void uploadUniformVec2(vec2 value, const char* uniformName);
//Upload vector of 3 floats
void uploadUniformVec3(vec3 value, const char* uniformName);
//Upload vector of 4 floats
void uploadUniformVec4(vec4 value, const char* uniformName);
//Upload 3 by 3 matrix
void uploadUniformMat3(mat3 value, const char* uniformName);
//Upload 4 by 4 matrix
void uploadUniformMat4(mat4 value, const char* uniformName);
