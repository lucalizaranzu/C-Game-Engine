#pragma once

#include <ShmingoCore.h>
#include "Vertex.h"

//Serves as just a wrapper around a std::array containing vertices, and eventually indices as well. 
//Future edits to this include animations and transforms.
//Look into submitting SSBOs to the gpu instead of animating and transforming actual vertex data
template<class VertexType, GLuint VertexAmt>
class Model{

public:

	inline GLuint getVertexCount() { return vertexCount; };
	inline std::array<VertexType, VertexAmt> getModelData() { return modelData; };

private:

	GLuint vertexCount;
	std::array<VertexType, VertexAmt> modelData;
};

