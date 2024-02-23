#version 420 core

in uvec2 pass_textureCoords;
in uint texID;

uniform sampler2D textures[1];

out vec4 FragColor;
void main(){

   FragColor = texture(textures[texID], pass_textureCoords);

};