#pragma once

#include <ShmingoCore.h>

#include "DataStructures.h"

namespace Shmingo {
	void declareTypeCorrespondence();
}

class InstancedEntity{

public:

	//----------------------------------------Front end code----------------------------------------------

	InstancedEntity(vec3 position, vec2 rotation, vec3 scale);


	virtual void update() = 0; //Update function for the entity, called every frame





	//----------------------------------------Engine-related code----------------------------------------//

	typedef std::vector<Shmingo::EntitySpecificInstanceDataInfo> perInstanceInstanceInfo;

	/// <summary>
	/// Returns pointer to beginning offset of the vertex data array corresponding to the desired attribute
	/// </summary>
	/// <param name="attributeNumber">
	/// attribute number
	/// </param>
	/// <param name="instancedAttributeInfo">
	/// Static instanced attribute array
	/// </param>
	/// <returns></returns>
	float* getInstanceData(GLuint attributeNumber);

	inline GLuint getOffsetInVao() { return offsetInVao; };
	inline void setOffsetInVao(GLuint offset) { offsetInVao = offset; }; //Used when adding entity vertex data to a VAO
	inline void decrementOffsetInVao() { offsetInVao--; }; //Used when deleting entity vertex data from a VAO	
	inline void incrementOffsetInVao() { offsetInVao++; }; //Used when adding entity vertex data to a VAO, probably not used

	virtual Shmingo::TransformComponent getTransformComponent() = 0; //Used for getters?
	virtual Shmingo::EntityType getEntityType() = 0;

	virtual perInstanceInstanceInfo getInstancedInstanceAttributeInfo() = 0;


protected:

	static constexpr GLuint instancedAttributeAmount = 0;

	/// <summary>
	/// To be used in constructor of derived classes to declare the per instance instance wide data for this entity
	/// </summary>
	/// <param name="localAttributeNumber">
	/// The LOCAL attribute number, offset at the first INSTANCED attribute number, not real attribute number. 
	/// If a VAO has 5 attributes, 3 normal, 2 instanced per vertex attributes and 2 instanced per instance vertex attributes, the local attribute number for the first instanced attribute is 0, not 5.
	/// </param>
	/// <param name="vertexData">
	/// Data to be passed to the VAO, such as position, rotation, etc.
	/// </param>
	void setInstanceData(GLuint localAttributeNumber, float data);
	void setInstanceData(GLuint localAttributeNumber, vec2 data);
	void setInstanceData(GLuint localAttributeNumber, vec3 data);
	void setInstanceData(GLuint localAttributeNumber, vec4 data);
	void setInstanceData(GLuint localAttributeNumber, mat4 data);


	/// <summary>
	/// Getters for the per instance instance data
	/// </summary>
	/// <param name="attributeNumber"></param>
	/// <returns></returns>
	float getInstanceDataFloat(GLuint attributeNumber);
	vec2 getInstanceDataVec2(GLuint attributeNumber);
	vec3 getInstanceDataVec3(GLuint attributeNumber);
	vec4 getInstanceDataVec4(GLuint attributeNumber);
	mat4 getInstanceDataMat4(GLuint attributeNumber);


	/// <summary>
	/// pass in the static attribute list to be used in the constructor (Should be the same for every derived class as double as you dont change the static member's name)
	/// </summary>
	/// <param name="vertexDataInfo">
	/// Class's static vertex attribute information list
	/// </param>
	void instantiateInstanceData();

	std::unique_ptr<float[]> perInstanceInstanceData; //Interleaved vertex data for this entity - will be converted by the getVertexData function to one attribute in a getter if data can be directly read as is


	GLuint offsetInVao = 0; //Offset of per instance vertex data (normalized to number of entities, not byte size)
};






constexpr void declareInstanceAttribute(std::vector<Shmingo::EntitySpecificVertexDataInfo>& attributeList, GLuint& offset, GLuint localIndex, const char* name, GLuint size) {
	attributeList.emplace(attributeList.begin() + localIndex, Shmingo::EntitySpecificVertexDataInfo(name, size, offset, localIndex));
}

GLuint getModelVertexCount(Shmingo::EntityType type);



/// <summary>
/// Sets the vertex attribute information for an entity type
/// </summary>
/// <param name="type">Type to set information for</param>
/// <param name="attributeAmount">Amount of attributes</param>
/// <param name="">Interleaved strings and unsigned ints corresponding to attribute name and amount of floats</param>
/// <returns></returns>
std::vector<Shmingo::EntitySpecificVertexDataInfo> createPerInstanceVertexDataList(Shmingo::EntityType type, GLuint attributeAmount, ...);

/// <summary>
/// Sets the instance attribute information for an entity type
/// </summary>
/// <param name="type">Type to set information for</param>
/// <param name="attributeAmount">Amount of attributes</param>
/// <param name="">Interleaved strings and unsigned ints corresponding to attribute name and amount of floats</param>
/// <returns></returns>
std::vector<Shmingo::EntitySpecificInstanceDataInfo> createPerInstanceInstanceDataList(Shmingo::EntityType type, GLuint attributeAmount, ...);



//Default entity used outside of instanced rendering, such as the batch renderer methods
//Just a rewrite of the original entity class
class DefaultEntity : public InstancedEntity {

public:

	virtual Shmingo::EntityType getEntityType() override { return Shmingo::DefaultEntity; };

	//Getters and setters applicable to all entity types


	void update() override;




	void setTransformationMatrix(mat4 newMatrix);

	void setPosition(vec3 newPosition);
	void setRotation(vec2 newRotation);
	void setScale(vec3 newScale);

	void setTextureID(float newTextureID);

	//Not using an instanced data getter for these two because our position and rotation data automatically put in a transformation matrix, they are stored separately
	vec3 getPosition() {return position;};
	vec2 getRotation() {return rotation;};
	float getTextureID();

	Shmingo::TransformComponent getTransformComponent() override;

	DefaultEntity(vec3 position, vec2 rotation, vec3 scale);

	static perInstanceInstanceInfo getInstancedUniformInfo() { return instancedAttributeInfo; };

	virtual perInstanceInstanceInfo getInstancedInstanceAttributeInfo() override {return instancedAttributeInfo;};


protected:

	void updateTransformationMatrix();

	static perInstanceInstanceInfo instancedAttributeInfo;

	//Used locally and for getters because we can't get directly from transform matrix
	vec3 position;
	vec2 rotation;
	vec3 scale;
};