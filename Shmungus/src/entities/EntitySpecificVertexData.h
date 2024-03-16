#pragma once

#include <sepch.h>

/*
Designed to be put in an array which is specific for each entity type.
Represents the layout of vertex data specific to a certain type of entity, such as wool color for sheep, or size for a slime.
This is probably going to be remade into an NBT tag, but for now it's just a struct.
*/
struct EntitySpecificVertexDataInfo {

	const char* name;
	GLuint size;
	GLuint localFloatOffset;
	GLuint vaoIndex;
};