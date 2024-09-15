#pragma once

#include <ShmingoCore.h>
#include "DataStructures.h"


const size_t MAX_CHUNK_POLYGONS = 16 * 16 * 40;

class ChunkVertexArray {

public:

ChunkVertexArray();


void bind(){ glBindVertexArray(vaoID); }

GLuint& getVaoID(){ return vaoID; }

GLsizei getAttribAmt() { return 3; } //3 attributes
size_t getInstanceCount() { return instanceAmount; }

void init();

void setUniforms();

/// <summary>
/// Function for submitting terrain data to the GPU
/// </summary>
/// <param name="positionsData">Byte compacted positions, format is: 4 bits for x, 4 bits for z, 8 bits for y</param>
/// <param name="ID">ID data for triangle, first 11 bits are for material ID, last 5 bits are for orientation ID</param>
/// <returns>Poly amount added</returns>
size_t submitTerrain(size_t offset, uint8_t* positionsData, unsigned short* ID, size_t polyAmount);

void submitTriangleDebug(size_t offset, uvec3 position, unsigned short materialID, uint8_t orientationID); //TODO delete this

private:

	GLuint vaoID;
	GLuint staticPositionsVboID;
	GLuint positionsVboID;
	GLuint IDVboID;


	size_t instanceAmount = 0;

};