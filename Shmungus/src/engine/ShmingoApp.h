#pragma once

#include <Engine.h>
#include "Input.h"
#include "ModelManager.h"

//This class is a singleton, there will only ever be one Application

namespace Shmingo {

	class ShmingoApp {

	public:

		~ShmingoApp();

		inline Shmingo::Window* getWindow() { return window; };

		inline static ShmingoApp& get() { return instance; };


		//Application wide information
		void declareEntiyType(EntityType type);

		std::vector<Shmingo::EntityType> entityTypes;

		void run();

	private:

		ShmingoApp(); //Privated constructor
		Shmingo::Window* window;

		void init();
		void update();

		static ShmingoApp instance;

	};

}