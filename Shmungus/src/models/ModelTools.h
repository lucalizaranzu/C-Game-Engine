#pragma once

#include <sepch.h>
#include <ShmingoCore.h>
#include "VertexTools.h"

#include "Model.h"
#include "TextureTools.h"

namespace Shmingo {
	Model createCubeModel(vec3 position, Texture2D texture);
	Model createQuadModel(vec3 position, Texture2D texture);

	/// <summary>
	/// Used to create a dynamically allocated model from a function that returns a model value
	/// </summary>
	/// <param name="modelCreatingFunction"> 
	/// Function which returns a model value
	/// </param>
	/// <returns>
	/// Pointer to dynamically allocated model
	/// </returns>
	Model* createModelPointer(std::function<Model()> modelCreatingFunction);


	std::unique_ptr<int[]> shiftIndices(int* indices, int amount, int indexCount);
}