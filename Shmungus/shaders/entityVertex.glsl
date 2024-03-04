#version 420 core

in vec3 positions;
in vec2 textureCoords;
in float textureID; 

layout(std140, binding = 0) uniform MatrixData {

	mat4 viewMatrix;
	mat4 projectionMatrix;

} matrices;

out vec2 pass_textureCoords;
flat out int texID;


void main()
{
	int intTex = int(textureID);
   gl_Position = matrices.projectionMatrix * matrices.viewMatrix * vec4(positions.x, positions.y, positions.z, 1.0);

   pass_textureCoords = textureCoords; //Setting the pass
   texID = intTex;
};

