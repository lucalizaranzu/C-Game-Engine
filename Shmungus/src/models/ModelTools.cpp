#include <sepch.h>
#include "ModelTools.h"


Model createCubeModel(vec3 position, Texture2D texture) {

  float* positions = new float[24]{
		//Front face
		-1.0f + position.x, 1.0f + position.y, 1.0f + position.z,
		-1.0f + position.x, -1.0f + position.y, 1.0f + position.z,
		1.0f + position.x, -1.0f + position.y, 1.0f + position.z,
		1.0f + position.x, 1.0f + position.y, 1.0f + position.z,
		//Back face
		-1.0f + position.x, 1.0f + position.y, -1.0f + position.z,
		-1.0f + position.x, -1.0f + position.y, -1.0f + position.z,
		1.0f + position.x, -1.0f + position.y, -1.0f + position.z,
		1.0f + position.x, 1.0f + position.y, -1.0f + position.z
	};

  float* texCoords = new float[16]{
      //Front face
      0.0f, 1.0f,
      0.0f, 0.0f,
      1.0f, 0.0f,
      1.0f, 1.0f,
          //Front face
      0.0f, 1.0f,
      0.0f, 0.0f,
      1.0f, 0.0f,
      1.0f, 1.0f,
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

   return Model(positions, texCoords, texture, 8, indices, 36);
}

Model createQuadModel(vec3 position, Texture2D texture) {

    float* positions = new float[12]{
		-1.0f + position.x, 1.0f + position.y, position.z,
		-1.0f + position.x, -1.0f + position.y, position.z,
		1.0f + position.x, -1.0f + position.y, position.z,
		1.0f + position.x, 1.0f + position.y, position.z
	};

    float* texCoords = new float[8]{
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f
    };

    int* indices = new int[6]{
        0,1,3,3,1,2
    };

    return Model(positions, texCoords, texture, 4, indices, 6);
}


//DO NOT FUCK WITH THIS FUNCTION! YOU WILL RANDOMLY CRASH AND WILL HAVE NO IDEA WHY, BUT IT WILL BE BECAUSE OF THE ASSIGNMENT OPERATOR OVERLOAD
Model* createModelPointer(std::function<Model()> modelCreatingFunction){

    Model* out = static_cast<Model*>(malloc(sizeof(Model))); //Bullshit that dynamically allocates memory for a model to write to

    out->instantiate();
    *out = modelCreatingFunction(); //Write to dynamically allocated memory

    return out;
}

std::unique_ptr<int[]> shiftIndices(int* indices, int amount, int indexCount) {
    std::unique_ptr<int[]> newIndices = std::make_unique<int[]>(indexCount);

    for (int i = 0; i < indexCount; i++) {
		newIndices.get()[i] = indices[i] + amount;
	}

	return newIndices; 
}
