#version 420 core

layout(location = 0) in vec3 staticPositions;
layout(location = 1) in ivec2 positions;   
layout(location = 2) in uint ID;


uniform mat4[64] transformArray;

out vec3 vertexColor; //Temporary for debugging

layout(std140) uniform Matrices {

	mat4 projectionMatrix;
	mat4 viewMatrix;
    mat4 ortho;

} matrices;

ivec2 decodeID() {
    // Extract the bits for each component
    //float r = float((encodedColor & uint(0x01)) * 0.25 + ((encodedColor >> 1) & uint(0x01)) * 0.75);
 
    uint orientID = ID & uint(0x3F); //Get least significant 6 bits 
    uint materialID = (ID >> 6) & uint(0x3FF); //Get most significant 10 bits

    // Return the decoded color
    return ivec2(materialID, orientID);
}

void main(){

    vertexColor = staticPositions;

    ivec2 IDs = decodeID();
    uint orientID = IDs.y; //ID unique to each triangle orientation
    uint materialID = IDs.x; //ID unique to each terrain material
    
	mat4 triangleTransformation = transformArray[orientID];

    vec4 vertexPosition = triangleTransformation * vec4(staticPositions,1.0f);
    vertexPosition.x += positions.x; //Add chunk x value
    vertexPosition.y += positions.y; //Add chunk y value

    gl_Position = vertexPosition;
}

