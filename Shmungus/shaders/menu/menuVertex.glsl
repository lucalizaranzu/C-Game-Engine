#version 420 core

layout(location = 0) in vec2 static_positions;
layout(location = 1) in vec2 transform_positions;
layout(location = 2) in vec2 transform_scale;
layout(location = 3) in uint textureID;

out vec2 texCoords;

uniform vec4 atlasTexCoords[16]; //Atlas texture coordinates (16 is arbitrary, if you need to store more than 16 textures in a quad then just change this)

layout(std140) uniform Matrices {

	mat4 projectionMatrix;
	mat4 viewMatrix;
    mat4 ortho;

} matrices;

void main(){

	vec4 texCoordsInfo = atlasTexCoords[textureID]; //Get current texture coordinates

	vec4 scaledPosition = vec4(static_positions.x * (transform_scale.x), static_positions.y * (transform_scale.y), 0,0);
	vec2 transformedPositions = scaledPosition.xy + transform_positions; //Translate scaled positions
	transformedPositions.y += (1 - transform_scale.y); //Flip y axis

	//Multiply left side by width, add pos.x, right side by height, add pos.y
	texCoords = vec2(static_positions.x * texCoordsInfo.x + texCoordsInfo.z, static_positions.y * texCoordsInfo.y + texCoordsInfo.w);
	//texCoords = static_positions;

	gl_Position = vec4(transformedPositions * 2 - 1,0,1);

}
