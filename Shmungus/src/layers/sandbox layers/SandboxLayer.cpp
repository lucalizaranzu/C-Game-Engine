#include <sepch.h>

#include "Layer.h"
#include "LayerStack.h"

#include "Player.h"
#include "MiscTools.h"
#include "TextureTools.h"
#include "ModelTools.h"
#include "Renderer.h"
#include "TextBox.h"


SandboxLayer::SandboxLayer() : world(World()) {
	type = Shmingo::SANDBOX_LAYER;
}

std::shared_ptr<Player> player;

std::shared_ptr<Model> cubeModel;

SandboxLayer::~SandboxLayer() {

}

int thingyIndex = 0;

void SandboxLayer::onAttach() {

	world.init();

	Texture2D funnyTexture = Shmingo::createTexture2D("funnyimage.png");

	cubeModel.reset(Shmingo::createModelPointer(std::bind(Shmingo::createCubeModel, vec3(0.0f, 0.0f, 0.0f), funnyTexture)));

	se_layerStack.addListener<SandboxLayer, KeyPressEvent>(Shmingo::SANDBOX_LAYER, this, &SandboxLayer::KeyboardCallback);

	//Init functions
	Shmingo::disableGLFWCursor();

	//Creating other objects

	player.reset(new Player(Shmingo::createCubeModel(vec3(0.0f,0.0f,0.0f),funnyTexture)));

	se_uniformBuffer.setAsActive();
	se_uniformBuffer.setProjectionMatrix(Shmingo::createViewMatrix(45.0f, 800, 600, 0.1f, 10000.0f));

	TextBox textBox = TextBox("This is a test to see how the lines functions work, lllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllll I am using correct english this time so I can identify what is going on", ivec2(1, 2), ivec2(250000, 100000), 4, 2);

}

void SandboxLayer::onUpdate() {

	player->update();
	world.update();

}

void SandboxLayer::KeyboardCallback(KeyPressEvent* e){

	if (e->getKey() == se_KEY_ESCAPE) {

		se_layerStack.emplaceOverlay(new PauseMenuLayer());
	}

	else if (e->getKey() == se_KEY_L) {
		world.createEntity(Shmingo::DefaultEntity, vec3(3 * (float)(thingyIndex % 6), 3 * (float)(thingyIndex / 6), -4.0f), vec2(0.0f,0.0f), vec3(1.2f,1.2f,1.2f));
		thingyIndex++;
	}

	else if (e->getKey() == se_KEY_K) {
		if (thingyIndex > 0) {
			world.deleteEntity(Shmingo::DefaultEntity, thingyIndex - 1);
			thingyIndex--;
		}
	}

	e->setHandled();
}

void SandboxLayer::onDetach() {
	cleanUp();
}

void SandboxLayer::cleanUp(){
	world.cleanUp();
}