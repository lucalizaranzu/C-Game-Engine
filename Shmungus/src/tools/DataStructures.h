#pragma once

#include <sepch.h>

using vec2 = glm::vec2; //Format ix x,y
using vec3 = glm::vec3; //Format is x,y,z
using vec4 = glm::vec4; //Format is x,y,z,w
using mat3 = glm::mat3x3;
using mat4 = glm::mat4x4;


namespace Shmingo {

	//Enums ---------------------------------------------------------------------------------------

	enum EntityType {

		DefaultEntity

	};

	enum LayerType {

		SANDBOX_LAYER,
		PAUSEMENU_LAYER

	};

	enum UniformBlock {

		MATRIX_BLOCK,
		UTIL_BLOCK
	};








	/*
	Designed to be put in an array which is specific for each entity type.
	Represents the layout of vertex data specific to a certain type of entity, such as wool color for sheep, or size for a slime.
	This is probably going to be remade into an NBT tag, but for now it's just a struct.
	*/
	struct EntitySpecificVertexDataInfo {

		const char* name;
		GLuint size; //Size of the attribute in number of floats
		GLuint localFloatOffset;
		GLuint vaoIndex;
	};


	/*
	Represents a transform for an entity
	Can move on 3 axes, and rotate on 2 axes
	*/
	struct TransformComponent {

		vec3 position;
		vec2 rotation;
	};

	/*
	Represents information about an entity type and its storage in an entity vector.
	Offset is the offset of the first entity of the given type in the entity vector
	Amount is the amount of entities of the given type in the entity vector
	*/
	struct EntityTypeInfo {

		uint8_t vertexArrayOffset; //Offset of vertex array in VAO map
		uint16_t offset; //Offset of first entity in entity map
		uint16_t amount; //Amount of entities of this type

	};


	/*
	Represents information about a uniform block in a uniform buffer
	*/
	struct UniformBlockInfo {

		const char* name;
		GLuint size;
		GLuint offset;
		GLuint bindingPoint;

	};	

	/// <summary>
	/// Represents a map that follows insertion order.
	/// </summary>
	/// <typeparam name="T1">
	/// Key type
	/// </typeparam>
	/// <typeparam name="T2">
	/// Value type
	/// </typeparam>
	template<typename T1, typename T2>
	class OrderedMap {

	public:

		std::vector<T1> keys;
		std::unordered_map<T1, T2> map;

		/// <summary>
		/// Inserts key into back of keys vector
		/// </summary>
		/// <param name="keyval">
		/// Key
		/// </param>
		/// <param name="value">
		/// Value
		/// </param>
		/// 
		void insert(T1 keyval, T2 value) {
			keys.emplace_back(keyval); //Emplace key into the back of the keys vector
			map.insert(std::make_pair(keyval, value)); //Insert key value pair into map
		}


		/// <summary>
		/// Inserts element into both key list and map
		/// </summary>
		/// <param name="it">
		/// Positional iterator
		/// </param>
		/// <param name="keyval">
		/// Key to insert
		/// </param>
		/// <param name="value">
		/// Value to insert with key
		/// </param>
		void insert(std::vector<T1>::iterator it, T1 keyval, T2 value) {
			keys.insert(it, keyval); //Insert key into keys vector at iterator position
			map.insert(std::make_pair(keyval, value)); //Insert key value pair into map
		}

		/// <summary>
		/// Retrieves value from map given key
		/// </summary>
		/// <param name="keyval">
		/// Key
		/// </param>
		/// <returns>
		/// Key's value
		/// </returns>
		T2 at(T1 keyval) {
			return map[keyval]; //Return value from map given key	
		}

		std::unordered_map<T1, T2>::iterator find(T1 keyval) {
			return map.find(keyval); //Return iterator to key value pair in map
		}

		/// <summary>
		/// Removes a key value pair
		/// </summary>
		/// <param name="keyval">
		/// Key to remove along with its value
		/// </param>
		void erase(T1 keyval) {
			map.erase(keyval); //Erase key value pair from map
			keys.erase(std::remove(keys.begin(), keys.end(), keyval), keys.end()); //Erase key from keys vector
		}
	};

	/// <summary>
	/// Represents a map that can be iterated over efficiently without relying on map lookups.
	/// Designed for when you need to be able to use map lookup functionality, but also need to frequently iterate over all values indiscriminately.
	/// </summary>
	/// <typeparam name="T">
	/// Type of value
	/// </typeparam>
	/// <typeparam name="E">
	/// Key type, typically going to be an enum or indexing type
	/// </typeparam>
	template<typename E, typename T>
	class IterableMap {

	public:

		std::vector<T> values;
		std::unordered_map<E, GLuint> indicesMap;

		void insert(E key, T value) {
			values.emplace_back(value);
			indicesMap.insert(std::make_pair(key, values.size() - 1));
		}

		/// <summary>
		/// Returns value 
		/// </summary>
		/// <param name="key"></param>
		/// <returns></returns>
		T at(E key) {
			return values[indicesMap[key]];
		}

		/// <summary>
		/// Removes a key value pair from the map and from values vector
		/// </summary>
		/// <param name="key">
		/// Key to remove
		/// </param>
		void remove(E key) {

			for (auto it = indicesMap.begin() + indicesMap.at(key); it != indicesMap.end(); it++) {
				it->second--;
			}

			values.erase(values.begin() + indicesMap[key]); //Remove from values vector
			indicesMap.erase(key); //Remove from indices map
		}

	};
}