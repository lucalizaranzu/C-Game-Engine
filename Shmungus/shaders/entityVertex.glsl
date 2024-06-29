#version 420 core

layout(location = 0) in vec3 positions;
layout(location = 1) in vec2 textureCoords;

layout(location = 2) in mat4 transformationMatrix;
layout(location = 6) in float textureID;

layout(std140) uniform Matrices {

	mat4 projectionMatrix;
	mat4 viewMatrix;
    vec3 padding;

} matrices;

layout(std140) uniform Util {
    
    vec3 padding3;
	float elapsedTime;

} util;


out vec2 pass_textureCoords;
flat out int texID;
out float time;

out vec3 outvect;

void main(){

    int intTex = int(textureID);
    gl_Position =  matrices.projectionMatrix * matrices.viewMatrix * transformationMatrix * vec4(positions.x, positions.y, positions.z, 1.0);

    pass_textureCoords = textureCoords; //Setting the pass
    texID = intTex;
    time = util.elapsedTime;


};