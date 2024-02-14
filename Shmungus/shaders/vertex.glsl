#version 330 core

layout (location = 0) in vec2 positions;
layout (location = 1) in vec4 color;

layout(std140) uniform MatrixData {

	mat4 viewMatrix;
	mat4 projectionMatrix;

} matrices;

out vec4 vColor;

void main()
{
   gl_Position = matrices.viewMatrix * matrices.projectionMatrix * vec4(positions.x, positions.y, 0.0f, 1.0);
   //vColor = vec3(positions.x + 0.5f,0.7f,positions.y + 0.7f);
   vColor = color;
};
