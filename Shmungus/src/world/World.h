#pragma once

#include <sepch.h>
#include <ShmingoCore.h>
#include <typeindex>

#include "InstancedEntity.h"
#include "InstancedVertexArray.h"

/*
Represents the world owned by the sandbox layer, including all of the expected constituents.
This class manages terrain, entities, and all game logic in the gameplay stage
*/
class World {

public:
	
	World();

	void submitVertexArrays(); //Submits all vertex arrays to the renderer

	/// <summary>
	/// Creates an entity based on type Shmingo::EntityType. Forward arguments required for entity constructor 
	/// </summary>
	/// <typeparam name="...Args">
	/// Forwarded arguments
	/// </typeparam>
	/// <param name="type">
	/// Type of Entity, see Shmingo::EntityType enum in InstancedEntity.h
	/// </param>
	/// <param name="position"> Position </param>
	/// <param name="rotation"> Rotation </param>
	/// <param name="...args">
	/// Forwarded arguments to specific constructor
	/// </param>
	template<typename... Args>
	void createEntity(Shmingo::EntityType type, vec3 position, vec2 rotation, Args&&... args) {

		InstancedEntity* e = nullptr; //return type

		switch (type) {

		case Shmingo::DefaultEntity:
			e = new DefaultEntity(position, rotation);
			break;

		default:
			e = nullptr;
			break;
		}


		addEntity(type, e); //Creates entity and adds to world

	}

	void deleteEntity(Shmingo::EntityType type, GLuint localOffset);

	void cleanUp();

	void init(); //Initializes world
	void update(); //Updates world


private:

	//-------------------------------Information about the world--------------------------------------------
	//Contains entity transforms in contiguous memory to reduce cache misses when looping
	std::vector<Shmingo::TransformComponent> entityTransformList;

	std::vector<InstancedEntity*> entityList; //Represents entities as a uint index to reduce cache misses. When looping, index this to get desired entity
	std::unordered_map<Shmingo::EntityType, bool> initializedInstancedVaoMap; //Map to keep track of which Entity types have a VAO in our list
	Shmingo::OrderedMap<Shmingo::EntityType, Shmingo::EntityTypeInfo> entityTypeInfoMap; //Contains information about a certain entity type in memory - ordered map is a custom data structure which retains insertion order

	std::unordered_map<Shmingo::EntityType, std::shared_ptr<InstancedVertexArray>> instancedVAOMap; //Contains all of the world's instanced VAOs, which typically includes entities.



	void addEntity(Shmingo::EntityType type, InstancedEntity* entity); //Is there any reason for this to be a pointer? TODO - check if this can be refactored to avoid dereferencing


};
