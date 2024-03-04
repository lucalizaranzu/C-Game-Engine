#version 420 core

in vec2 pass_textureCoords;
flat in int texID;

uniform sampler2D textures[32];

out vec4 FragColor;

void main(){

   FragColor = texture(textures[texID],pass_textureCoords);

};