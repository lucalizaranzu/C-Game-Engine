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

	vec2 scaledPosition = vec4(matrices.ortho * vec4(static_positions,0,0) * vec4(transform_scale,0,0)).xy; //Scale static positions
	vec2 transformedPositions = scaledPosition + transform_positions; //Translate scaled positions	

	vec4 currentTexCoordsInfo = atlasTexCoords[textureID]; //Get current texture coordinates

	texCoords = mix(currentTexCoordsInfo.xy, currentTexCoordsInfo.zw, vec2(static_positions.x, 1.0 - static_positions.y)); //Mix texture coordinates

	gl_Position = vec4(transformedPositions, 0.0, 1.0); //Set quad position

}
