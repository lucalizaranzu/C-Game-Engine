#pragma once

#include <ShmingoCore.h>
#include <sepch.h>

#include "Model.h"
#include "InstancedEntity.h"

//This class is for when we want to use instancing instead of batching, mostly for entities
//Templated to entity type for optimizations

class InstancedVertexArray {

public:

	//Constructor, sets up all necessary data
	InstancedVertexArray(Shmingo::EntityType entityType, std::shared_ptr<Model> model);





	//Vertex data functions ---------------------------------------------------
	/// <summary>
	/// Submits vertex data for instanced data
	/// Takes main vertex data from model, and instanced data from entity
	/// Templated class, so entity instanced data changes based on the templated form of this class
	/// </summary>
	/// <typeparam name="E"> Templated entity class, useful for static information</typeparam>
	/// <param name="entity"> Entity object to upload </param>
	void submitInstanceData(std::shared_ptr<InstancedEntity> instancedEntity);
	void submitInstanceData(InstancedEntity* instancedEntity);

	/// <summary>
	/// Updates the instance data of an entity in the VAO
	/// </summary>
	/// <param name="entity">Reference to entity</param>
	/// <param name="attributePositionInArray">Index of the information of an attribute in the attrib info array (If an entity has attributes mat4, float, pass a 1 to 
	/// update the float even though a mat4 takes up 4 attributes)</param>
	void updateInstanceData(InstancedEntity* entity, GLuint attributePositionInArray);

	/// <summary>
	/// Removes entity data from the VAO and fills its slot with the last entity in the VAO
	/// </summary>
	/// <param name="offset"></param>
	void removeInstancedData(GLuint offset);



	//Getters ------------------------------------------------------------------
	inline GLuint getVaoID() { return vaoID; };
	inline GLuint getVertexCount() { return vertexCount; };
	inline GLuint getIndexCount() { return indexCount; };
	inline GLuint getInstanceAmount() { return instanceAmount; };

	inline size_t getAttribAmount() { return attribAmount; };




	//Texture related functions ------------------------------------------------
	/// <summary>
	/// Links a texture object to a slot
	/// </summary>
	/// <param name="texture">Texture object to be linked</param>
	/// <param name="slot">Slot to link texture to</param>
	void linkTexture(Texture2D texture, GLuint slot);

	/// <summary>
	/// Binds textures : to be called in render method
	/// </summary>
	void bindTextures();

	inline Shmingo::EntityType getEntityType() {
		return entityType;
	}






	//Debugging functions ----------------------------------------------
	GLuint* getVaoIDRef() { return &vaoID; };

protected:

	Shmingo::EntityType entityType;

	size_t attribAmount = 0;

	GLuint vaoID = 0; //VAO ID
	GLuint eboID = 0; //Index Buffer ID

	GLuint vertexPositionsVboID = 0;
	GLuint texCoordsVboID = 0;

	std::vector<GLuint> perInstanceVboIDs; //Per instance uniform VBO IDs
	std::vector<Texture2D> textureList;

	GLuint vertexCount = 0; //Amount of vertices
	GLuint instanceAmount = 0;
	GLuint indexCount = 0; //Amount of indices
	GLuint maxTextureIndex = 0;

	std::shared_ptr<Model> instanceModel; //The model that this Vertex Array is based on
	std::vector<Shmingo::EntitySpecificInstanceDataInfo>& perInstanceAttributeInfo;

	//Utility functions --------------------------------------------------------

	inline void bindVao() {
		glBindVertexArray(vaoID);
	}
	inline void bindIndicesVbo(){
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);
	}
	inline void unbindBuffer() {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	/// <summary>
	/// Copies part of a buffer to another part of the same buffer
	/// </summary>
	/// <param name="readOffset">
	/// Offset where data will be read from
	/// </param>
	/// <param name="writeOffset">
	/// Offset where data will be written to
	/// </param>
	/// <param name="size">
	/// Size of data to be copied in bytes
	/// </param>
	void copyBufferData(GLuint readOffset, GLuint writeOffset, GLuint size);
};