#version 420 core

layout (location = 0) in vec3 positions;
layout (location = 1) in uvec2 textureCoords;
layout (location = 2) in uint textureID; 

layout(std140, binding = 0) uniform MatrixData {

	mat4 viewMatrix;
	mat4 projectionMatrix;

} matrices;

out vec4 vColor;
out vec2 pass_textureCoords;
out uint texID;


void main()
{
   gl_Position = matrices.projectionMatrix * matrices.viewMatrix * vec4(positions.x, positions.y, -1.0f, 1.0);

   pass_textureCoords = textureCoords; //Setting the pass
   texID = textureID;
};

