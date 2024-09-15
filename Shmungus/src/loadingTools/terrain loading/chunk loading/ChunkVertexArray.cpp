#include <sepch.h>

#include "ChunkVertexArray.h"
#include "Matrices.h"
#include "MasterRenderer.h"



ChunkVertexArray::ChunkVertexArray(){

	float staticPositions[9] = {
		0.0f,0.0f,0.0f,
		1.0f,1.0f,0.0f,
		0.0f,1.0f,0.0f
	};


	glGenVertexArrays(1, &vaoID);

	glGenBuffers(1, &staticPositionsVboID);
	glGenBuffers(1, &positionsVboID);
	glGenBuffers(1, &IDVboID);

	bind();

	glBindBuffer(GL_ARRAY_BUFFER, staticPositionsVboID);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), &staticPositions, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // transform position
	glVertexAttribDivisor(0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, positionsVboID);
	glBufferData(GL_ARRAY_BUFFER, MAX_CHUNK_POLYGONS * 2, nullptr, GL_DYNAMIC_DRAW);
	glVertexAttribIPointer(1, 2, GL_UNSIGNED_BYTE, 2 * sizeof(uint8_t), (void*)0); // transform position
	glVertexAttribDivisor(1, 1);

	glBindBuffer(GL_ARRAY_BUFFER, IDVboID);
	glBufferData(GL_ARRAY_BUFFER, MAX_CHUNK_POLYGONS * 2, nullptr, GL_DYNAMIC_DRAW);
	glVertexAttribIPointer(2, 1, GL_UNSIGNED_SHORT, 2, (void*)0); // transform position
	glVertexAttribDivisor(2, 1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void ChunkVertexArray::init(){
	//TODO set uniforms after declaring a terrain shader in the master renderer
	setUniforms();
}


size_t ChunkVertexArray::submitTerrain(size_t offset, uint8_t* positionsData, unsigned short* ID, size_t polyAmount){
	bind();

	glBindBuffer(GL_ARRAY_BUFFER, positionsVboID);
	glBufferSubData(GL_ARRAY_BUFFER, offset * 2 * sizeof(uint8_t), polyAmount * 2 * sizeof(uint8_t), positionsData);


	glBindBuffer(GL_ARRAY_BUFFER, IDVboID);
	glBufferSubData(GL_ARRAY_BUFFER, offset * sizeof(unsigned short), polyAmount * sizeof(unsigned short), ID);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	instanceAmount++;

	return polyAmount;
}

void ChunkVertexArray::submitTriangleDebug(size_t offset, uvec3 position, unsigned short materialID, uint8_t orientationID){

	unsigned short ID = (materialID << 6); //Start by shifting material ID left 6 bits
	se_log("ID part 1: " << ID);
	ID = ID | ((orientationID & 0x3F)); //Emplace orientation over
	se_log("ID part 2: " << ID);


	uint8_t positionXZ = 0;

	if (position.x < 16 && position.z < 16) {
		uint8_t x = (uint8_t)position.x;
		uint8_t z = (uint8_t)position.z;

		positionXZ = (x << 4) | z;
	}
	else {
		se_error("X or Y out of bounds when submitting triangle with position " << position.x, ", " << position.z << ", " << position.y);
		return;
	}

	uint8_t positionsData[2] = { positionXZ, (uint8_t)position.y };

	submitTerrain(offset, positionsData, &ID, 1);
}



void ChunkVertexArray::setUniforms(){

	std::shared_ptr<ShaderProgram> terrainShader = se_masterRenderer.getShader(se_TERRAIN_SHADER);

	mat4* transformationsArray = new mat4[64]; //64 matrices for transforming triangles

	transformationsArray[0] = Shmingo::createTransformationMatrix(vec3(0, 0, 0), vec3(0, 0, 0), vec3(1, 1, 1)); // Triangle 1
	transformationsArray[1] = Shmingo::createTransformationMatrix(glm::vec3(1, 1, 0), glm::vec3(0, 0, glm::radians(180.0f)), glm::vec3(1, 1, 1)); // Triangle 2
	// Right face  
	transformationsArray[2] = Shmingo::createTransformationMatrix(glm::vec3(1, 0, 0), glm::vec3(0, glm::radians(90.0f), 0), glm::vec3(1, 1, 1)); // Triangle 3
	transformationsArray[3] = Shmingo::createTransformationMatrix(glm::vec3(1, 1, -1), glm::vec3(0, glm::radians(90.0f), glm::radians(180.0f)), glm::vec3(1, 1, 1)); // Triangle 4
	// Back face
	transformationsArray[4] = Shmingo::createTransformationMatrix(glm::vec3(1, 0, -1), glm::vec3(0, glm::radians(180.0f), 0), glm::vec3(1, 1, 1)); // Triangle 5
	transformationsArray[5] = Shmingo::createTransformationMatrix(glm::vec3(0, 1, -1), glm::vec3(0, glm::radians(180.0f), glm::radians(180.0f)), glm::vec3(1, 1, 1)); // Triangle 6
	// Left face  
	transformationsArray[6] = Shmingo::createTransformationMatrix(glm::vec3(0, 0, -1), glm::vec3(0, glm::radians(270.0f), 0), glm::vec3(1, 1, 1)); // Triangle 3
	transformationsArray[7] = Shmingo::createTransformationMatrix(glm::vec3(0, 1, 0), glm::vec3(0, glm::radians(270.0f), glm::radians(180.0f)), glm::vec3(1, 1, 1)); // Triangle 4
	// Top face
	transformationsArray[8] = Shmingo::createTransformationMatrix(glm::vec3(0, 1, 0), glm::vec3(glm::radians(-90.0f), 0, 0), glm::vec3(1, 1, 1)); // Triangle 7
	transformationsArray[9] = Shmingo::createTransformationMatrix(glm::vec3(1, 1, -1), glm::vec3(glm::radians(-90.0f), 0, glm::radians(180.0f)), glm::vec3(1, 1, 1)); // Triangle 8
	// Bottom face
	transformationsArray[10] = Shmingo::createTransformationMatrix(glm::vec3(0, 0, -1), glm::vec3(glm::radians(90.0f), 0, 0), glm::vec3(1, 1, 1)); // Triangle 9
	transformationsArray[11] = Shmingo::createTransformationMatrix(glm::vec3(1, 0, 0), glm::vec3(glm::radians(90.0f), 0, glm::radians(180.0f)), glm::vec3(1, 1, 1)); // Triangle 10

	//Alternates
	transformationsArray[12] = Shmingo::createTransformationMatrix(vec3(0, 1, 0), vec3(glm::radians(180.0f), 0, 0), vec3(1, 1, 1)); // Triangle 1
	transformationsArray[13] = Shmingo::createTransformationMatrix(glm::vec3(1, 0, 0), glm::vec3(glm::radians(180.0f), 0, glm::radians(180.0f)), glm::vec3(1, 1, 1)); // Triangle 2
	// Right face  
	transformationsArray[14] = Shmingo::createTransformationMatrix(glm::vec3(1, 0, -1), glm::vec3(0, glm::radians(270.0f), 0), glm::vec3(1, 1, 1)); // Triangle 3
	transformationsArray[15] = Shmingo::createTransformationMatrix(glm::vec3(1, 1, 0), glm::vec3(0, glm::radians(270.0f), glm::radians(180.0f)), glm::vec3(1, 1, 1)); // Triangle 4
	// Back face
	transformationsArray[16] = Shmingo::createTransformationMatrix(glm::vec3(1, 1, -1), glm::vec3(0, 0.0f, glm::radians(180.0f)), glm::vec3(1, 1, 1)); // Triangle 5
	transformationsArray[17] = Shmingo::createTransformationMatrix(glm::vec3(0, 0, -1), glm::vec3(0, 0.0f, 0), glm::vec3(1, 1, 1)); // Triangle 6
	// Left face  
	transformationsArray[18] = Shmingo::createTransformationMatrix(glm::vec3(0, 0, 0), glm::vec3(glm::radians(180.0f), glm::radians(90.0f), glm::radians(180.0f)), glm::vec3(1, 1, 1)); // Triangle 3
	transformationsArray[19] = Shmingo::createTransformationMatrix(glm::vec3(0, 1, -1), glm::vec3(glm::radians(180.0f), glm::radians(90.0f), 0), glm::vec3(1, 1, 1)); // Triangle 4
	// Top face
	transformationsArray[20] = Shmingo::createTransformationMatrix(glm::vec3(0, 1, -1), glm::vec3(glm::radians(90.0f), 0, 0), glm::vec3(1, 1, 1)); // Triangle 7
	transformationsArray[21] = Shmingo::createTransformationMatrix(glm::vec3(1, 1, 0), glm::vec3(glm::radians(90.0f), 0, glm::radians(180.0f)), glm::vec3(1, 1, 1)); // Triangle 8
	// Bottom face
	transformationsArray[22] = Shmingo::createTransformationMatrix(glm::vec3(0, 0, 0), glm::vec3(glm::radians(270.0f), 0, 0), glm::vec3(1, 1, 1)); // Triangle 9
	transformationsArray[23] = Shmingo::createTransformationMatrix(glm::vec3(1, 0, -1), glm::vec3(glm::radians(270.0f), 0, glm::radians(180.0f)), glm::vec3(1, 1, 1)); // Triangle 10

	transformationsArray[24] = Shmingo::createTransformationMatrix(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1)); //Normal diagonal 1
	transformationsArray[25] = Shmingo::createTransformationMatrix(glm::vec3(1, 0, 0), glm::vec3(0, glm::radians(90.0f), 0), glm::vec3(1, 1, 1)); // Normal Diagonal 2
	transformationsArray[26] = Shmingo::createTransformationMatrix(glm::vec3(1, 0, -1), glm::vec3(0, glm::radians(180.0f), 0), glm::vec3(1, 1, 1)); // Normal Diagonal 3
	transformationsArray[27] = Shmingo::createTransformationMatrix(glm::vec3(0, 0, -1), glm::vec3(0, glm::radians(270.0f), 0), glm::vec3(1, 1, 1)); // Normal Diagonal 4

	transformationsArray[28] = Shmingo::createTransformationMatrix(glm::vec3(1, 1, -1), glm::vec3(glm::radians(180.0f), glm::radians(90.0f), 0), glm::vec3(1, 1, 1)); // Upside down diagonal 1
	transformationsArray[29] = Shmingo::createTransformationMatrix(glm::vec3(0, 1, -1), glm::vec3(glm::radians(180.0f), glm::radians(0.0f), 0), glm::vec3(1, 1, 1)); // Upside down diagonal 2
	transformationsArray[30] = Shmingo::createTransformationMatrix(glm::vec3(0, 1, 0), glm::vec3(glm::radians(180.0f), glm::radians(270.0f), 0), glm::vec3(1, 1, 1)); // Upside down diagonal 3
	transformationsArray[31] = Shmingo::createTransformationMatrix(glm::vec3(1, 1, 0), glm::vec3(glm::radians(180.0f), glm::radians(180.0f), 0), glm::vec3(1, 1, 1)); // Upside down diagonal 4

	transformationsArray[32] = Shmingo::createTransformationMatrix(vec3(0, 0, 0), vec3(glm::radians(-45.0f), 0, 0), vec3(1, 1.41421356f, 1)); // Triangle 1
	transformationsArray[33] = Shmingo::createTransformationMatrix(vec3(1, 1, -1), vec3(glm::radians(-45.0f), 0, glm::radians(180.0f)), vec3(1, 1.41421356f, 1)); // Triangle 1

	auto transformsLocation = glGetUniformLocation(terrainShader->getProgramID(), "transformArray");
	terrainShader->start();
	glUniformMatrix4fv(transformsLocation, 64, false, reinterpret_cast<float*>(transformationsArray));
	terrainShader->stop();

	delete[] transformationsArray;
}