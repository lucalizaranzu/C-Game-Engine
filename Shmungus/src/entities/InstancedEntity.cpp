#include <sepch.h>
#include "InstancedEntity.h"
#include "ModelTools.h"
#include "TextureTools.h"
#include "MasterRenderer.h"


//Initialize static variables
std::vector<Shmingo::EntitySpecificVertexDataInfo> DefaultEntity::instancedAttributeInfo = DefaultEntity::setInstancedAttribInfo();


InstancedEntity::InstancedEntity(vec3 position, vec2 rotation){}


void InstancedEntity::declareVertexData(GLuint localAttributeNumber, InstancedAttributeInformation& attribInfo, float data) {
	auto currentAttribInfo = attribInfo[localAttributeNumber];
	vertexData.get()[currentAttribInfo.localFloatOffset] = data;
}

void InstancedEntity::declareVertexData(GLuint localAttributeNumber, InstancedAttributeInformation& attribInfo, vec2 data) {
	auto currentAttribInfo = attribInfo[localAttributeNumber];
	vertexData.get()[currentAttribInfo.localFloatOffset] = data.x;
	vertexData.get()[currentAttribInfo.localFloatOffset + 1] = data.y;
}

void InstancedEntity::declareVertexData(GLuint localAttributeNumber, InstancedAttributeInformation& attribInfo, vec3 data) {
	auto currentAttribInfo = attribInfo[localAttributeNumber];
	vertexData.get()[currentAttribInfo.localFloatOffset] = data.x;
	vertexData.get()[currentAttribInfo.localFloatOffset + 1] = data.y;
	vertexData.get()[currentAttribInfo.localFloatOffset + 2] = data.z;
}

void InstancedEntity::declareVertexData(GLuint localAttributeNumber, InstancedAttributeInformation& attribInfo, vec4 data) {
	auto currentAttribInfo = attribInfo[localAttributeNumber];
	vertexData.get()[currentAttribInfo.localFloatOffset] = data.x;
	vertexData.get()[currentAttribInfo.localFloatOffset + 1] = data.y;
	vertexData.get()[currentAttribInfo.localFloatOffset + 2] = data.z;
	vertexData.get()[currentAttribInfo.localFloatOffset + 3] = data.w;
}

void InstancedEntity::instantiateVertexData(InstancedAttributeInformation& vertexDataInfo) {

	GLuint size = 0;
	for (Shmingo::EntitySpecificVertexDataInfo& info : vertexDataInfo) {
		size += info.size;
	}
	vertexData = std::make_unique<float[]>(size);
}

//Functions for all entity types

inline float* InstancedEntity::getVertexData(GLuint attributeNumber, InstancedAttributeInformation& instancedAttributeInfo){
	return &vertexData.get()[instancedAttributeInfo[attributeNumber].localFloatOffset];
}

void DefaultEntity::setPosition(vec3 newPosition){
	declareVertexData(0, instancedAttributeInfo, newPosition);
}

void DefaultEntity::setRotation(vec2 newRotation){
	declareVertexData(1, instancedAttributeInfo, newRotation);
}

void DefaultEntity::setTextureID(float newTextureID){
	declareVertexData(2, instancedAttributeInfo, newTextureID);
}

vec3 DefaultEntity::getPosition(){
	float* positions = getVertexData(0, instancedAttributeInfo);
	return vec3(positions[0],positions[1],positions[2]);
}

vec2 DefaultEntity::getRotation(){
	float* rotation = getVertexData(1, instancedAttributeInfo);
	return vec2(rotation[0], rotation[1]);
}

float DefaultEntity::getTextureID() {
	return getVertexData(2, instancedAttributeInfo)[0];
}

Shmingo::TransformComponent DefaultEntity::getTransformComponent(){
	Shmingo::TransformComponent transform;

	transform.position = getPosition();
	transform.rotation = getRotation();

	return transform;
}

//DefaultEntity is a base implementation of InstancedEntity containing basic vertex attributes that all entities should have.
//All subclasses will still have to manually declare the instanced attributes in the setInstancedAttribInfo method, however they will have
//access to all of the getters and setters for that data without needing to redefine it.

DefaultEntity::DefaultEntity(vec3 position, vec2 rotation) : InstancedEntity(position,rotation){

	instantiateVertexData(instancedAttributeInfo);

	setPosition(position);
	setRotation(rotation);
	setTextureID(0);
}

std::vector<Shmingo::EntitySpecificVertexDataInfo> DefaultEntity::setInstancedAttribInfo() {
	std::vector<Shmingo::EntitySpecificVertexDataInfo> vertexDataInfo;
	GLuint sizeCounter = 0;

	declareInstanceAttribute(vertexDataInfo, sizeCounter, 0, "position", 3);
	declareInstanceAttribute(vertexDataInfo, sizeCounter, 1, "rotation", 2);
	declareInstanceAttribute(vertexDataInfo, sizeCounter, 2, "textureID", 1);

	se_masterRenderer.declareEntitySpecificAttribAmount(Shmingo::DefaultEntity, 3); //Declares 3 custom attribute slots for DefaultEntity
	se_masterRenderer.declareEntityVertexAttributes(Shmingo::DefaultEntity, vertexDataInfo); //Declares vertex attributes for other classes to access
	return vertexDataInfo;
}