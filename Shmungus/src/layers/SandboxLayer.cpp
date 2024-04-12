#include <sepch.h>

#include "Layer.h"
#include "LayerStack.h"

#include "Player.h"
#include "MiscTools.h"
#include "TextureTools.h"
#include "ModelTools.h"
#include "Renderer.h"


SandboxLayer::SandboxLayer() : world(World()) {
	type = Shmingo::SANDBOX_LAYER;
}

std::shared_ptr<Player> player;

std::shared_ptr<Model> cubeModel;

SandboxLayer::~SandboxLayer() {

}

void SandboxLayer::onAttach() {

	world.init();

	Texture2D funnyTexture = Shmingo::createTexture2D("funnyimage.png");

	cubeModel.reset(Shmingo::createModelPointer(std::bind(Shmingo::createCubeModel, vec3(0.0f, 0.0f, 0.0f), funnyTexture)));

	se_layerStack.addListener<SandboxLayer, KeyPressEvent>(Shmingo::SANDBOX_LAYER, this, &SandboxLayer::KeyboardCallback);

	//Init functions
	Shmingo::disableGLFWCursor();

	//Creating other objects

	player.reset(new Player(Shmingo::createCubeModel(vec3(0.0f,0.0f,0.0f),funnyTexture)));
	world.createEntity(Shmingo::DefaultEntity, vec3(0.0f, 0.0f, -2.0f), vec3(0.0f, 0.0f, 0.0f));


	se_uniformBuffer.setAsActive();
	se_uniformBuffer.setProjectionMatrix(createProjectionMatrix(45.0f, 800, 600, 0.1f, 10000.0f));

}

void SandboxLayer::onUpdate() {

	player->update();
	world.update();

}

void SandboxLayer::KeyboardCallback(KeyPressEvent* e){

	if (e->getKey() == se_KEY_ESCAPE) {

		se_layerStack.emplaceOverlay(new PauseMenuLayer());
	}
	e->setHandled();
}

void SandboxLayer::onDetach() {
	cleanUp();
}

void SandboxLayer::cleanUp(){
	world.cleanUp();
}