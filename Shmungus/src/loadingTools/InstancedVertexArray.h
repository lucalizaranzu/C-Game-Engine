#pragma once

#include <ShmingoCore.h>
#include <sepch.h>

#include "Model.h"
#include "InstancedEntity.h"

//This class is for when we want to use instancing instead of batching, mostly for entities
//Templated to entity type for optimizations

template<class E> //E for entity because only entities should be used to template this class
class InstancedVertexArray {

public:

	//Constructor, sets up all necessary data
	inline InstancedVertexArray(std::shared_ptr<Model> model) : instanceModel(model) {

		attribAmount = 3 + E::instancedAttributeAmount; //Set total number of attributes to be bound and unbound in the render method

		GLuint modelVertexCount = model->getVertexCount();

		glGenVertexArrays(1, &vaoID); //Creates VAO using member variable

		glGenBuffers(1, &vertexPositionsVboID); //Generates a vertex buffer for positions
		glGenBuffers(1, &texCoordsVboID); //Generates a vertex buffer for texCoords
		glGenBuffers(1, &eboID); //Generates a vertex buffer for indices

		glBindVertexArray(vaoID); //Bind VAO

		glBindBuffer(GL_ARRAY_BUFFER, vertexPositionsVboID);
		glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(float) * modelVertexCount, instanceModel->getPositionData(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

		glBindBuffer(GL_ARRAY_BUFFER, texCoordsVboID);
		glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(float) * modelVertexCount, instanceModel->getTextureCoords(), GL_STATIC_DRAW);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, instanceModel->getIndexCount() * sizeof(int), instanceModel->getIndexData(), GL_STATIC_DRAW);

		GLuint instancedAttribAmount = E::instancedAttributeAmount;

		auto instancedAttributeInfo = E::getInstancedAttributeInfo();

		for (GLuint i = 0; i < instancedAttribAmount; i++) {

			EntitySpecificVertexDataInfo currentAttributeInfo = instancedAttributeInfo[i];
			instancedVboIDs[i] = 0;
			glGenBuffers(1, &instancedVboIDs[i]); //Generates a vertex buffer for instanced attributes
			glBindBuffer(GL_ARRAY_BUFFER, instancedVboIDs[i]); //Bind vertex buffer

			glBufferData(GL_ARRAY_BUFFER, MAX_FLOATS * sizeof(float) * currentAttributeInfo.size, nullptr, GL_DYNAMIC_DRAW); //Sets buffer data arguments for vertex buffer
			glVertexAttribPointer(2 + currentAttributeInfo.vaoIndex, currentAttributeInfo.size, GL_FLOAT, GL_FALSE, currentAttributeInfo.size * sizeof(float), nullptr);
			glVertexAttribDivisor(2 + currentAttributeInfo.vaoIndex, 1); //Sets vertex attribute divisor to once per instance
		}

		glBindVertexArray(0); //Unbind vertex buffer 
	}

	/// <summary>
	/// Submits vertex data for instanced data
	/// Takes main vertex data from model, and instanced data from entity
	/// Templated class, so entity instanced data changes based on the templated form of this class
	/// </summary>
	/// <typeparam name="E"> Templated entity class, useful for static information</typeparam>
	/// <param name="entity"> Entity object to upload </param>
	inline void submitInstanceData(std::shared_ptr<E> instancedEntity) {

		glBindVertexArray(vaoID); //Bind VAO

		const GLuint instancedAttribAmt = E::instancedAttributeAmount;

		instancedEntity->setOffsetInVao(instanceAmount);

		bindVao();

		std::vector<EntitySpecificVertexDataInfo> instancedAttributeInfo = E::getInstancedAttributeInfo();

		for (GLuint i = 0; i < instancedAttribAmt; i++) {

			float* instancedData = instancedEntity->getVertexData(i, instancedAttributeInfo);

			glBindBuffer(GL_ARRAY_BUFFER, instancedVboIDs[i]); //Bind vertex buffer
			glBufferSubData(GL_ARRAY_BUFFER, instancedEntity->getOffsetInVao() * instancedAttributeInfo[i].size * sizeof(float),
				instancedAttributeInfo[i].size * sizeof(float), instancedData); //Sets buffer data arguments for vertex buffer	
		}

		instanceAmount += 1;
		indexCount += instanceModel->getIndexCount();
	}

	//Getters
	inline GLuint getVaoID() { return vaoID; };
	inline GLuint getVertexCount() { return vertexCount; };
	inline GLuint getIndexCount() { return indexCount; };
	inline GLuint getInstanceAmount() { return instanceAmount; };

	inline GLuint getAttribAmount() { return attribAmount; };


	/// <summary>
	/// Links a texture object to a slot
	/// </summary>
	/// <param name="texture">Texture object to be linked</param>
	/// <param name="slot">Slot to link texture to</param>
	inline void linkTexture(Texture2D texture, GLuint slot) {

		//Declares the size of the array so we don't have to always loop through 32 slots
		if ((slot + 1) > maxTextureIndex) {
			maxTextureIndex = slot + 1;
		}

		//Inserts texture ID into array with index being slot. can be overriden
		auto it = textureList.begin();
		textureList.emplace(std::next(it, slot), texture);
		texture.setVaoLocalSlot(slot);
		//std::cout << "ID in link: ";
	}

	/// <summary>
	/// Binds textures : to be called in render method
	/// </summary>
	inline void bindTextures() {
		bindVao();
		//Loops through all the texture IDs and binds them to their slot according to their index in the array
		for (int i = 0; i < (int)maxTextureIndex; i++) {
			textureList.at(i).bind();
		}
	}

protected:

	GLuint attribAmount = 0;

	GLuint vaoID = 0; //VAO ID
	GLuint eboID = 0; //Index Buffer ID

	GLuint vertexPositionsVboID = 0;
	GLuint texCoordsVboID = 0;

	std::array<GLuint, E::instancedAttributeAmount> instancedVboIDs; //Per instance VBO IDs
	std::vector<Texture2D> textureList;

	GLuint vertexCount = 0; //Amount of vertices
	GLuint instanceAmount = 0;
	GLuint indexCount = 0; //Amount of indices
	GLuint maxTextureIndex = 0;

	std::shared_ptr<Model> instanceModel; //The model that this Vertex Array is based on

	inline void bindVao() {
		glBindVertexArray(vaoID);
	}
	inline void bindIndicesVbo(){
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);
	}
};