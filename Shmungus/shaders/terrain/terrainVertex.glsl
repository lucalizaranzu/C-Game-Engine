#version 420 core

layout(location = 0) in vec3 staticPositions;
layout(location = 1) in uvec2 positions;   
layout(location = 2) in uint ID;


uniform mat4[64] transformArray;

out vec3 vertexColor; //Temporary for debugging

layout(std140) uniform Matrices {

	mat4 projectionMatrix;
	mat4 viewMatrix;
    mat4 ortho;

} matrices;

ivec2 decodeID() {
 
    uint orientID = ID & uint(0x3F); //Get least significant 6 bits 
    uint materialID = (ID >> 6) & uint(0x3FF); //Get most significant 10 bits

    // Return the decoded color
    return ivec2(materialID, orientID);
}

ivec3 decodePositions(){

    uint x = (positions.x >> 4 & (0x0F));
    uint z = (positions.x & (0x0F));
    uint y = positions.y;

    return ivec3(x,y,z);
}



void main(){

    vertexColor = staticPositions;

    uvec2 IDs = decodeID();
    uint orientID = IDs.y; //ID unique to each triangle orientation
    uint materialID = IDs.x; //ID unique to each terrain material

    vec3 decodedPosition = decodePositions();

    vec3 pass_staticPositions = staticPositions;

    if(orientID >= 24 && orientID <= 31){ //Condition where we need to transform our base mesh
        if(gl_VertexID == 0){ pass_staticPositions.x += 1;} //Move vertex 0 to the right by 1
        else if(gl_VertexID == 1){pass_staticPositions.z -= 1; } //Move vertex 1 back by 1
    }

    
	mat4 triangleTransformation = transformArray[orientID]; //Get transformation matrix

    vec4 translatedPositions = (triangleTransformation * vec4(pass_staticPositions, 1.0) + vec4(decodedPosition,1)); //transform then translate triangle



    gl_Position = matrices.projectionMatrix * matrices.viewMatrix * translatedPositions;
}

