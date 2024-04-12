#include "sepch.h"
#include "World.h"

#include "MasterRenderer.h"
#include "Renderer.h"
#include "ShmingoApp.h"
#include "ModelTools.h"
#include "TextureTools.h"

World::World() {

}

void World::init(){

	//Initialize the entity type info map with all entity types
	for (Shmingo::EntityType type : se_application.entityTypes) {
		initializedInstancedVaoMap.insert(std::make_pair(type, false));
	}
}

void World::update(){

	submitVertexArrays();
}





//Other functions

void World::submitVertexArrays(){
 	for (auto it = instancedVAOMap.begin(); it != instancedVAOMap.end(); it++) {
		Shmingo::renderInstanced(it->second, se_masterRenderer.getEntityShader(it->first));
	}

}

void World::addEntity(Shmingo::EntityType type, InstancedEntity* entity){
	//If entity type does not exist in the entity type map, create a new entity type info and VAO
	if (initializedInstancedVaoMap.at(type) == false) {
		//Creates info for new entity type. Sets the offset to the current size of the entity list, amount is 0 because will increment after conditional
		entityTypeInfoMap.insert(type, Shmingo::EntityTypeInfo(entityList.size(), 0)); 

		instancedVAOMap.insert(std::make_pair(type, std::make_shared<InstancedVertexArray>(type, se_masterRenderer.getEntityModel(type)))); //Create new VAO for entity type
		initializedInstancedVaoMap[type] = true; //Set entity type to initialized
	}

	std::shared_ptr<InstancedVertexArray> entityVAO = instancedVAOMap.at(type); //Get VAO for entity type

	//Emplace entity into entity list at the offset of the entity type
	entityList.emplace(entityList.begin() + entityTypeInfoMap.map[type].offset, entity); //Inserts entity into entity list at the offset of the entity type
	//Submit entity data to the VAO
	entityVAO->submitInstanceData(entity);


	//Update information about all succeeding entity types
	for(auto it = entityTypeInfoMap.keys.begin() + entityTypeInfoMap.map[type].offset + 1; it != entityTypeInfoMap.keys.end(); it++){
		entityTypeInfoMap.map[*it].offset++; //Increment offset of all succeeding entity types
	}
	entityTypeInfoMap.map[type].amount++; //Increment amount of entities of the given type
}



void World::cleanUp(){

	for (InstancedEntity* entity : entityList) {
		delete entity;
	}
}
