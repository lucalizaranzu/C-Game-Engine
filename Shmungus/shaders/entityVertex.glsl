#version 420 core

in vec3 positions;
in vec2 textureCoords;

layout(location = 2) in vec3 instancePositions;
layout(location = 4) in float textureID;

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
out float addedPositions;
flat out int texID;
out float time;

mat4 createTranslationMatrix(vec3 translation) {
    return mat4(
        vec4(1.0, 0.0, 0.0, 0.0),
        vec4(0.0, 1.0, 0.0, 0.0),
        vec4(0.0, 0.0, 1.0, 0.0),
        vec4(translation, 1.0)
    );
}

void main(){

    mat4 transformation = createTranslationMatrix(instancePositions);

	int intTex = int(textureID);
   gl_Position = matrices.projectionMatrix * matrices.viewMatrix * transformation * vec4(positions.x, positions.y, positions.z, 1.0);

   pass_textureCoords = textureCoords; //Setting the pass
   texID = intTex;
   time = util.elapsedTime;
   addedPositions = instancePositions.x + instancePositions.y;
};