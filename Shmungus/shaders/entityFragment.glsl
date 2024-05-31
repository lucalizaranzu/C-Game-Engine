#version 420 core

in vec2 pass_textureCoords;
flat in int texID;
in float time;

uniform sampler2D textures[32];

out vec4 FragColor;

void main(){

   //FragColor = texture(textures[texID],pass_textureCoords);
   FragColor = vec4(abs(sin(time)), abs(cos(time)), cos(time), 1.0f);

};