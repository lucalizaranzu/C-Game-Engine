#version 420 core

in vec3 positions;
in vec2 textureCoords;

layout(location = 2) in vec3 instancePositions;
layout(location = 4) in float textureID;

layout(std140, binding = 0) uniform MatrixData {

	mat4 viewMatrix;
	mat4 projectionMatrix;

} matrices;

out vec2 pass_textureCoords;
flat out int texID;
out float test;

mat4 createTranslationMatrix(vec3 translation) {
    return mat4(
        vec4(1.0, 0.0, 0.0, 0.0),
        vec4(0.0, 1.0, 0.0, 0.0),
        vec4(0.0, 0.0, 1.0, 0.0),
        vec4(translation, 1.0)
    );
}


void main()
{

    mat4 transformation = createTranslationMatrix(instancePositions);

	int intTex = int(textureID);
   gl_Position = matrices.projectionMatrix * matrices.viewMatrix * transformation * vec4(positions.x, positions.y, positions.z, 1.0);

   pass_textureCoords = textureCoords; //Setting the pass
   texID = intTex;
   test = instancePositions.x;
};

