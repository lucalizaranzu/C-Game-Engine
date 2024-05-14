#version 420 core

in vec2 pass_textureCoords;
flat in int texID;
in float time;
in float addedPositions;

uniform sampler2D textures[32];

out vec4 FragColor;

void main(){

   //FragColor = texture(textures[texID],pass_textureCoords);
   FragColor = vec4(abs(sin(time * addedPositions / 15.0f)), abs(cos(time * addedPositions / 15.0f)), cos(time * addedPositions / 10.0f), 1.0f);

};