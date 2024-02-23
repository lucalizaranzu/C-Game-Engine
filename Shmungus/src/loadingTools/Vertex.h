#pragma once
#include <ShmingoCore.h>

#define MAX_FLOATS 10000 //Maximum amount of floats per VAO

//To specify what type of vertex array to create
enum class VERTEX_TYPE {

	DEFAULT_VERTEX,
	POSITIONS_ONLY_VERTEX,
	QUAD_VERTEX,
	QUAD_VERTEX_COLOR
};

struct DefaultVertex{

	float positions[3];
	float texCoords[2];
	float normals[3];
};

struct PositionsOnlyVertex{

	float positions[3];
};

struct TexturedQuadVertex {

	float positions[2];
	float texCoords[2];
};

struct ColorQuadVertex {

	float positions[2];
	float color[4];
};


//Real vertices
struct EntityVertex {

	float positions[3];
	byte texCoords[2];
	byte textureID;

};
