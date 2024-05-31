#pragma once

#include <Engine.h>
#include <typeindex>
#include "Input.h"
#include "ModelManager.h"
#include "World.h"

//This class is a singleton, there will only ever be one Application

namespace Shmingo {

	class ShmingoApp {

	public:

		~ShmingoApp();

		inline Shmingo::Window* getWindow() { return window; };
		inline World* getCurrentWorld() { return currentWorld; };
		inline float getDeltaTime() { return deltaTime; }
		inline double getTimeElapsed() { return timeElapsed; }
		inline bool getOnTick() { return onTick; }

		void setCurrentWorld(World* world);

		inline static ShmingoApp& get() { return instance; };


		//Application wide information
		void declareEntityType(std::type_index typeIndex, EntityType type); //Type index not needed anymore but going to keep to make sure all entity type enums are real types

		std::vector<Shmingo::EntityType> entityTypes;
		void run();

		//Global getters
		inline int getMinimumUniformBlockOffset() { return minimumUniformBlockOffset; }

	private:

		ShmingoApp(); //Privated constructor

		Shmingo::Window* window; //Window object
		World* currentWorld; //Reference to current world object

		void init();
		void update();

		static ShmingoApp instance;

		double deltaTime = 0; //Time since last frame
		double timeElapsed = 0; //Time since start of application
		bool onTick = false;

		//Global variables
		void initGlobalVariables();
		int minimumUniformBlockOffset = 0; //Minimum offset for uniform blocks

		//Application wide information

	};

}