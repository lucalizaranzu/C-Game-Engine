#include <sepch.h>
#include "ModelTools.h"


Model createCubeModel(vec3 position, float textureID) {

    if (textureID == -1.0f) {
        std::cerr << "Could not create model given texture ID : " << textureID << "Check vertex array!" << std::endl;
    }

  EntityVertex* vertexData = new EntityVertex[8]{
        //Front face
        EntityVertex{-1.0f + position.x,1.0f + position.y,1.0f + position.z,  0.0f,1.0f, textureID},
        EntityVertex{-1.0f + position.x,-1.0f + position.y,1.0f + position.z,   0.0f,0.0f, textureID},
        EntityVertex{1.0f + position.x,-1.0f + position.y,1.0f + position.z,   1.0f,0.0f, textureID},
        EntityVertex{1.0f + position.x,1.0f + position.y,1.0f + position.z,    1.0f,1.0f, textureID},
        EntityVertex{-1.0f + position.x,1.0f + position.y,-1.0f + position.z,  0.0f,1.0f, textureID},
        EntityVertex{-1.0f + position.x,-1.0f + position.y,-1.0f + position.z,   0.0f,0.0f, textureID},
        EntityVertex{1.0f + position.x,-1.0f + position.y,-1.0f + position.z,   1.0f,0.0f, textureID},
        EntityVertex{1.0f + position.x,1.0f + position.y,-1.0f + position.z,    1.0f,1.0f, textureID},
    };

    int* indices = new int[36]{
        //Front
        0, 1, 3,
        3, 1, 2,

        //Left
        4, 5, 0,
        0, 5, 1,

        //Bottom
        1, 5, 2,
        2, 5, 6,

        //Right
        3, 2, 7,
        7, 2, 6,

        //Top
        4, 0, 7,
        7, 0, 3,

        //Back
        7, 6, 4,
        4, 6, 5
    };

    return Model(vertexData, 8, indices, 36, (GLuint) textureID);
}

Model createQuadModel(vec3 position, float textureID) {

    //Front face
    EntityVertex* vertexData = new EntityVertex[8]{
        EntityVertex{ -1.0f + position.x,1.0f + position.y,position.z,  0.0f,1.0f, textureID },
        EntityVertex{ -1.0f + position.x,-1.0f + position.y,position.z,   0.0f,0.0f, textureID },
        EntityVertex{ 1.0f + position.x,-1.0f + position.y,position.z,   1.0f,0.0f, textureID },
        EntityVertex{ 1.0f + position.x,1.0f + position.y,position.z,    1.0f,1.0f, textureID },
    };

    int* indices = new int[6]{
        0,1,3,3,1,2
    };

    return Model(vertexData, 4, indices, 6, (GLuint) textureID);
}


//DO NOT USE THIS OUTSIDE OF SPECIFIED VERTEX ARRAY FUNCTIONS! THIS WILL CAUSE A MEMORY LEAK!
std::unique_ptr<int[]> shiftIndices(int* indices, int amount, int indexCount) {
    std::unique_ptr<int[]> newIndices = std::make_unique<int[]>(indexCount);

    for (int i = 0; i < indexCount; i++) {
		newIndices.get()[i] = indices[i] + amount;
	}

	return newIndices; 
}
