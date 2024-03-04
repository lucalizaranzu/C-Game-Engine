#pragma once

#include <ShmingoCore.h>
#include "VertexTools.h"

#include "Model.h"
#include "TextureTools.h"

Model createCubeModel(vec3 position, float textureID);
Model createQuadModel(vec3 position, float textureID);


std::unique_ptr<int[]> shiftIndices(int* indices, int amount, int indexCount);