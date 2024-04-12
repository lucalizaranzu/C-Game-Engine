#pragma once

#include <ShmingoCore.h>

#include "DataStructures.h"


class InstancedEntity{

public:

	typedef std::vector<Shmingo::EntitySpecificVertexDataInfo> InstancedAttributeInformation;

	//Actual information

	InstancedEntity(vec3 position, vec3 rotation);

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
	inline float* getVertexData(GLuint attributeNumber, InstancedAttributeInformation& instancedAttributeInfo);

	inline GLuint getOffsetInVao() { return offsetInVao; };
	inline void setOffsetInVao(GLuint offset) { offsetInVao = offset; }; //Used when adding entity vertex data to a VAO


protected:

	static constexpr GLuint instancedAttributeAmount = 0;


	/// <summary>
	/// To be used in constructor of derived classes to declare the vertex data for this entity
	/// </summary>
	/// <param name="localAttributeNumber">
	/// The LOCAL attribute number, offset at the first INSTANCED attribute number, not real attribute number. 
	/// If a VAO has 5 attributes, 3 normal and 2 instanced, the local attribute number for the first instanced attribute is 0, not 3.
	/// </param>
	/// <param name="vertexData">
	/// Data to be passed to the VAO, such as position, rotation, etc.
	/// </param>
	void declareVertexData(GLuint localAttributeNumber, InstancedAttributeInformation& attribInfo, float data);
	void declareVertexData(GLuint localAttributeNumber, InstancedAttributeInformation& attribInfo, vec2 data);
	void declareVertexData(GLuint localAttributeNumber, InstancedAttributeInformation& attribInfo, vec3 data);
	void declareVertexData(GLuint localAttributeNumber, InstancedAttributeInformation& attribInfo, vec4 data);

	/// <summary>
	/// pass in the static attribute list to be used in the constructor (Should be the same for every derived class as long as you dont change the static member's name)
	/// </summary>
	/// <param name="vertexDataInfo">
	/// Class's static vertex attribute information list
	/// </param>
	void instantiateVertexData(InstancedAttributeInformation& vertexDataInfo);

	std::unique_ptr<float[]> vertexData; //Interleaved vertex data for this entity - will be converted by the getVertexData function to one attribute
	static InstancedAttributeInformation attributeList; //List of all vertex data for this entity

	GLuint offsetInVao = 0; //Offset of per instance vertex data (normalized to number of entities, not byte size)
};






constexpr void declareInstanceAttribute(std::vector<Shmingo::EntitySpecificVertexDataInfo>& attributeList, GLuint& counter, GLuint localIndex, const char* name, GLuint size) {
	attributeList.emplace(attributeList.begin() + localIndex, Shmingo::EntitySpecificVertexDataInfo(name, size, counter, localIndex));
	counter += size;
}






//Default entity used outside of instanced rendering, such as the batch renderer methods
//Just a rewrite of the original entity class
class DefaultEntity : public InstancedEntity {

public:

	//Getters and setters applicable to all entity types
	void setPosition(vec3 newPosition);
	void setRotation(float newRotation);
	void setTextureID(float newTextureID);

	vec3 getPosition();
	float getRotation();
	float getTextureID();

	static InstancedAttributeInformation setInstancedAttribInfo();

	DefaultEntity(vec3 position, vec3 rotation);

	static InstancedAttributeInformation getInstancedAttributeInfo() { return instancedAttributeInfo; }; //Cant override since its technically not virtual, but should work in template context

protected:

	static InstancedAttributeInformation instancedAttributeInfo;
};