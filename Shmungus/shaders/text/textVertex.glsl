#version 420 core

layout (location = 0) in vec2 positions; // <vec2 pos>

layout (location = 1) in vec2 texCoords;
layout (location = 2) in vec2 instancePosition;
layout (location = 3) in uint textureID;
layout (location = 4) in uint color;
layout (location = 5) in uint scale;

out VertexData{
    vec2 pass_texCoords;
    flat uint pass_textureID;
    flat uint pass_Color;
    bool pass_skip;
}vertexData;

layout(std140) uniform Matrices {

	mat4 projectionMatrix;
	mat4 viewMatrix;
    mat4 ortho;

} matrices;


void main(){

    if((color & uint(0x80)) == uint(0x80)){ //Checks the skip bit
		gl_Position = vec4(0.0f,0.0f,0.0f,0.0f);
        vertexData.pass_skip=true;
	}

    else{
        
        vertexData.pass_skip = false;

        vertexData.pass_texCoords=texCoords;
        vertexData.pass_texCoords.y = 1.0f - vertexData.pass_texCoords.y;

        vertexData.pass_textureID = textureID;
        vertexData.pass_Color=color;

        vec4 scaledPosition = matrices.ortho * vec4((int(scale) * positions).xy,0.0f,0.0f);

        float x = ((scaledPosition.x / 200) + instancePosition.x) * 2 - 1;
        float y = ((scaledPosition.y / 200) + instancePosition.y) * 2 + 1;


        gl_Position = vec4(x,y, 0.0f, 1.0f);
    }
}