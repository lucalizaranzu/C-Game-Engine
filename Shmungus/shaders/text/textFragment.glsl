#version 330 core

out vec4 color;

in VertexData{
    vec2 pass_texCoords;
    flat uint pass_textureID;
    flat uint pass_Color;
}vertexData;

uniform sampler2DArray font;

vec3 decodeColor(uint encodedColor) {
    // Extract the bits for each component
    float r = float((encodedColor & uint(0x01)) * 0.25 + ((encodedColor >> 1) & uint(0x01)) * 0.75);
    float g = float(((encodedColor >> 2) & uint(0x01)) * 0.25 + ((encodedColor >> 3) & uint(0x01)) * 0.75);
    float b = float(((encodedColor >> 4) & uint(0x01)) * 0.25 + ((encodedColor >> 5) & uint(0x01)) * 0.75);

    // Return the decoded color
    return vec3(r, g, b);
}

void main() {
    vec4 sampled = vec4(decodeColor(vertexData.pass_Color), texture(font, vec3(vertexData.pass_texCoords.xy, vertexData.pass_textureID)).r);
    
    if(sampled.a < 0.1) {
        discard; // Discard the fragment if the alpha is less than 0.1
    } else {
        color = sampled; // Otherwise, use the sampled color
    }
}




