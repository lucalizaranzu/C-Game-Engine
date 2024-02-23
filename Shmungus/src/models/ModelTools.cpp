#include <sepch.h>
#include "ModelTools.h"

Model createCubeModel(std::string texturePath) {

    EntityVertex vertexData[8] = {
        //Front face
        {0.0f, 0.0f, 0.0f, {0, 0}, 0},
        {1.0f, 0.0f, 0.0f, {1, 0}, 0},
        {1.0f, 1.0f, 0.0f, {1, 1}, 0},
        {0.0f, 1.0f, 0.0f, {0, 1}, 0},
        // Back face
        {0.0f, 0.0f, 1.0f, {0, 0}, 0},
        {1.0f, 0.0f, 1.0f, {1, 0}, 0},
        {1.0f, 1.0f, 1.0f, {1, 1}, 0},
        {0.0f, 1.0f, 1.0f, {0, 1}, 0}
    };

    int indices[36] = {
        // Front face
        0, 1, 2,
        2, 3, 0,
        // Back face
        4, 5, 6,
        6, 7, 4,
        // Top face
        3, 2, 6,
        6, 7, 3,
        // Bottom face
        0, 1, 5,
        5, 4, 0,
        // Left face
        0, 3, 7,
        7, 4, 0,
        // Right face
        1, 2, 6,
        6, 5, 1
    };

    Texture texture = createTexture2D(("assets/textures/" + texturePath).c_str());

    return Model(vertexData, 8, indices, 36, texture);
}
