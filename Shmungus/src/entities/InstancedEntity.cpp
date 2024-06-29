#include <sepch.h>
#include "InstancedEntity.h"
#include "ShmingoApp.h"
#include "ModelTools.h"
#include "TextureTools.h"
#include "MasterRenderer.h"
#include "World.h"
#include "Matrices.h"
#include "InstancedVertexArray.h"


//-------------------------------------------------Utility Functions-------------------------------------------------
void Shmingo::declareTypeCorrespondence(){
	se_application.declareEntityType(typeid(DefaultEntity), Shmingo::DefaultEntity);
}

std::vector<Shmingo::EntitySpecificVertexDataInfo> createPerInstanceVertexDataList(Shmingo::EntityType type, GLuint attributeAmount, ...) {

    va_list args;
    va_start(args, attributeAmount);

    std::vector<Shmingo::EntitySpecificVertexDataInfo> out;
    GLuint currentAttrib = 0;
    GLuint offset = 0;

    GLuint totalAttributes = 0;

    for (GLuint  i = 0; i < attributeAmount; ++i) {
        const char* name = va_arg(args, const char*);
        int size = va_arg(args, GLuint);

        out.emplace_back(name, size, offset, currentAttrib); //Add attribute to list

        if (size > 4) {
            GLuint subAttribAmount = size / 4;
            GLuint remainder = size % 4;

            totalAttributes += subAttribAmount - 1; //-1 because we add one to currentAttrib at the end of the loop
            currentAttrib += subAttribAmount - 1;

            if (remainder != 0) {
				totalAttributes++;
                currentAttrib++;
			}
		}

        offset += size * getModelVertexCount(type); //Set offset for next attribute
        currentAttrib++; //Increment attribute number
        totalAttributes++;
    }

    va_end(args);
    return out;
}

std::vector<Shmingo::EntitySpecificInstanceDataInfo> createPerInstanceInstanceDataList(Shmingo::EntityType type, GLuint attributeAmount, ...) {

    va_list args;
    va_start(args, attributeAmount);

    std::vector<Shmingo::EntitySpecificInstanceDataInfo> out;
    GLuint currentAttrib = 0;
    GLuint offset = 0;

    GLuint totalAttributes = 0;

    for (GLuint i = 0; i < attributeAmount; ++i) {
        const char* name = va_arg(args, const char*);
        int size = va_arg(args, GLuint);

        out.emplace_back(name, size, offset, currentAttrib); //Add attribute to list

        if (size > 4) {
            GLuint subAttribAmount = size / 4;
            GLuint remainder = size % 4;

            totalAttributes += subAttribAmount - 1; //-1 because we add one to currentAttrib at the end of the loop
            currentAttrib += subAttribAmount - 1;

            if (remainder != 0) {
                totalAttributes++;
                currentAttrib++;
            }
        }

        offset += size; //Set offset for next attribute
        currentAttrib++; //Increment attribute number
        totalAttributes++;
    }

    se_masterRenderer.declareEntityInstanceAttributes(type, out); //Declares attribNumber amount of custom attribute slots for DefaultEntity)
    se_masterRenderer.declareEntitySpecificInstanceAttribAmount(type, attributeAmount, totalAttributes); //Declares amount of custom attributes

    va_end(args);
    return out;
}

GLuint getModelVertexCount(Shmingo::EntityType type){
   return se_masterRenderer.getEntityModel(type)->getVertexCount();
}


//-------------------------------------------------Entity Initializations-------------------------------------------------
std::vector<Shmingo::EntitySpecificInstanceDataInfo> DefaultEntity::instancedAttributeInfo = createPerInstanceInstanceDataList(Shmingo::DefaultEntity, 2, "transformationMatrix", 16, "textureID", 1);

//-------------------------------------------------Entity Functions-------------------------------------------------
InstancedEntity::InstancedEntity(vec3 position, vec2 rotation, vec3 scale){}

void InstancedEntity::instantiateInstanceData() {
	GLuint size = 0;
	for (Shmingo::EntitySpecificInstanceDataInfo& info : getInstancedInstanceAttributeInfo()) {
		size += info.size;
	}
	perInstanceInstanceData = std::make_unique<float[]>(size);
}


//-------------------------------------------------Setters-------------------------------------------------


void InstancedEntity::setInstanceData(GLuint localAttributeNumber, float data) {
    auto currentAttribInfo = getInstancedInstanceAttributeInfo()[localAttributeNumber];
    perInstanceInstanceData.get()[currentAttribInfo.localFloatOffset] = data;
}

void InstancedEntity::setInstanceData(GLuint localAttributeNumber, vec2 data) {
    auto currentAttribInfo = getInstancedInstanceAttributeInfo()[localAttributeNumber];
    vec2* dest = reinterpret_cast<vec2*>(&perInstanceInstanceData.get()[currentAttribInfo.localFloatOffset]);
    *dest = data;
}

void InstancedEntity::setInstanceData(GLuint localAttributeNumber, vec3 data) {
    auto currentAttribInfo = getInstancedInstanceAttributeInfo()[localAttributeNumber];
    vec3* dest = reinterpret_cast<vec3*>(&perInstanceInstanceData.get()[currentAttribInfo.localFloatOffset]);
    *dest = data;
}

void InstancedEntity::setInstanceData(GLuint localAttributeNumber, vec4 data) {
    auto currentAttribInfo = getInstancedInstanceAttributeInfo()[localAttributeNumber];
    vec4* dest = reinterpret_cast<vec4*>(&perInstanceInstanceData.get()[currentAttribInfo.localFloatOffset]);
    *dest = data;
}

void InstancedEntity::setInstanceData(GLuint localAttributeNumber, mat4 data) {
    auto currentAttribInfo = getInstancedInstanceAttributeInfo()[localAttributeNumber];
    mat4* dest = reinterpret_cast<mat4*>(&perInstanceInstanceData.get()[currentAttribInfo.localFloatOffset]);
    *dest = data;
}



//-------------------------------------------------Getters-------------------------------------------------



float* InstancedEntity::getInstanceData(GLuint attributeNumber) {
    return &perInstanceInstanceData.get()[getInstancedInstanceAttributeInfo()[attributeNumber].localFloatOffset];
}


float InstancedEntity::getInstanceDataFloat(GLuint attributeNumber) {
    if (getInstancedInstanceAttributeInfo()[attributeNumber].size != 1) {
		std::cerr << "Attribute " << getInstancedInstanceAttributeInfo()[attributeNumber].name << " is not a float" << std::endl;
        return 0;
	}
    return getInstanceData(attributeNumber)[0];
}
vec2 InstancedEntity::getInstanceDataVec2(GLuint attributeNumber) {
    if (getInstancedInstanceAttributeInfo()[attributeNumber].size != 2) {
        std::cerr << "Attribute " << getInstancedInstanceAttributeInfo()[attributeNumber].name << " is not a vec2" << std::endl;
        return vec2(0,0);
    }
    return vec2(getInstanceData(attributeNumber)[0], getInstanceData(attributeNumber)[1]);
}
vec3 InstancedEntity::getInstanceDataVec3(GLuint attributeNumber) {
    if (getInstancedInstanceAttributeInfo()[attributeNumber].size != 3) {
		std::cerr << "Attribute " << getInstancedInstanceAttributeInfo()[attributeNumber].name << " is not a vec3" << std::endl;
		return vec3(0,0,0);
	}
    return vec3(getInstanceData(attributeNumber)[0], getInstanceData(attributeNumber)[1], getInstanceData(attributeNumber)[2]);
}
vec4 InstancedEntity::getInstanceDataVec4(GLuint attributeNumber) {
    if (getInstancedInstanceAttributeInfo()[attributeNumber].size != 4) {
        std::cerr << "Attribute " << getInstancedInstanceAttributeInfo()[attributeNumber].name << " is not a vec4" << std::endl;
        return vec4(0, 0, 0, 0);
    }
    return vec4(getInstanceData(attributeNumber)[0], getInstanceData(attributeNumber)[1], getInstanceData(attributeNumber)[2], getInstanceData(attributeNumber)[3]);
}
mat4 InstancedEntity::getInstanceDataMat4(GLuint attributeNumber) {
    if (getInstancedInstanceAttributeInfo()[attributeNumber].size != 16) {
		std::cerr << "Attribute " << getInstancedInstanceAttributeInfo()[attributeNumber].name << " is not a mat4" << std::endl;
		return mat4(1);
	}
    mat4 out;
    std::copy(getInstanceData(attributeNumber), getInstanceData(attributeNumber) + 16, &out[0][0]);
    return out;
}



//-------------------------------------------------Implementations-------------------------------------------------

//DefaultEntity is a base implementation of InstancedEntity containing basic vertex attributes that all entities should have.
//All subclasses will still have to manually declare the instanced attributes in the setInstancedAttribInfo method, however they will have
//access to all of the getters and setters for that data without needing to redefine it.

DefaultEntity::DefaultEntity(vec3 position, vec2 rotation, vec3 scale) : InstancedEntity(position, rotation, scale), position(position), rotation(rotation), scale(scale) {

	instantiateInstanceData();

    auto matrix = Shmingo::createTransformationMatrix(position, rotation, scale);

    setTransformationMatrix(matrix);

	setTextureID(0);

}

void DefaultEntity::update() {
}







void DefaultEntity::updateTransformationMatrix(){
    mat4 newMatrix = Shmingo::createTransformationMatrix(position, rotation, scale);
    setTransformationMatrix(newMatrix);
    se_currentWorld->instancedVAOMap[Shmingo::DefaultEntity]->updateInstanceData(this, 0);
}

void DefaultEntity::setPosition(vec3 newPosition){
	position = newPosition;
    updateTransformationMatrix();
}

void DefaultEntity::setRotation(vec2 newRotation){
	rotation = newRotation;
    updateTransformationMatrix();
}

void DefaultEntity::setScale(vec3 newScale) {
	scale = newScale;
	updateTransformationMatrix();
}

void DefaultEntity::setTextureID(float newTextureID){
	setInstanceData(1, newTextureID);
}

void DefaultEntity::setTransformationMatrix(mat4 newMatrix){
	setInstanceData(0, newMatrix);
}

float DefaultEntity::getTextureID() {
	return getInstanceDataFloat(1);
}

Shmingo::TransformComponent DefaultEntity::getTransformComponent(){
	Shmingo::TransformComponent transform;

	transform.position = position;
	transform.rotation = rotation;

	return transform;
}