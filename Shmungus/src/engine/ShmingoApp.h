#pragma once

#include <Engine.h>
#include <typeindex>
#include "Input.h"
#include "ModelManager.h"
#include "World.h"
#include "FontUtil.h"

//This class is a singleton, there will only ever be one Application

namespace Shmingo {

	class ShmingoApp {

	public:

		~ShmingoApp();

		inline Shmingo::Window* getWindow() { return window; };
		inline World* getCurrentWorld() { return currentWorld; };
		inline double getDeltaTime() { return deltaTime; }
		inline double getTimeElapsed() { return timeElapsed; }
		inline bool getOnTick() { return onTick; }
		inline std::string getApplicationInfo(std::string key) { return applicationInfo[key]; }
		inline Shmingo::Character getCharacterFontInfo(GLchar c) { return charMap[c]; }

		inline bool isInApplicationInfo(std::string key) { return applicationInfo.find(key) != applicationInfo.end(); }

		void setCurrentWorld(World* world);

		inline static ShmingoApp& get() { return instance; };


		//Application wide information
		void declareEntityType(std::type_index typeIndex, EntityType type); //Type index not needed anymore but going to keep to make sure all entity type enums are real types
		void declareCharacterFontInfo(GLchar c, Shmingo::Character character) { charMap[c] = character; };

		//Set the font texture array ID
		void setFontTextureArrayID(std::string font, GLuint newID);
		//Get a reference to the ID of the font texture array
		GLuint* getFontTextureArrayIDPtr(std::string font) { return &fontTextureArrayMap[font]; }
		GLuint getFontTextureArrayID(std::string font) { return fontTextureArrayMap[font]; }
		

		std::vector<Shmingo::EntityType> entityTypes;
		void run();

		//Global getters
		inline int getMinimumUniformBlockOffset() { return minimumUniformBlockOffset; }

	private:

		ShmingoApp(); //Privated constructor

		void init();
		void update();

		Shmingo::Window* window; //Window object
		World* currentWorld; //Reference to current world object

		static ShmingoApp instance;



		//Global variables
		void initGlobalVariables();
		int minimumUniformBlockOffset = 0; //Minimum offset for uniform blocks
		double deltaTime = 0; //Time since last frame
		double timeElapsed = 0; //Time since start of application
		bool onTick = false;

		//Global OpenGL objects

		//Application wide information

		void setApplicationInfo(std::string key, std::string value);

		std::unordered_map<std::string, std::string> applicationInfo;
		std::unordered_map<GLchar, Shmingo::Character> charMap;

		std::unordered_map<std::string, std::unordered_map<GLchar, Shmingo::Character>> fontMap;
		std::unordered_map<std::string, GLuint> fontTextureArrayMap;
	};
}