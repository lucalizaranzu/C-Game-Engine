#version 420 core

in vec2 pass_textureCoords;
flat in int texID;
in float time;

uniform sampler2D textures[32];

out vec4 FragColor;

void main(){

   vec4 texColor = texture(textures[texID],pass_textureCoords);
   vec4 colorColor = vec4(abs(sin(time)), abs(cos(time)), cos(time), 1.0f);

   FragColor = vec4(mix(texColor.xyz,colorColor.xyz, 0.5f),1.0f);
};