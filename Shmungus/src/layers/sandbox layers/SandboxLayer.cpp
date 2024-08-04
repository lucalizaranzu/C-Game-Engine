#include <sepch.h>

#include "Layer.h"
#include "LayerStack.h"

#include "Player.h"
#include "MiscTools.h"
#include "TextureTools.h"
#include "ModelTools.h"
#include "Renderer.h"
#include "TextBox.h"
#include "TextVertexArray.h"


SandboxLayer::SandboxLayer() : world(World()) {
	type = Shmingo::SANDBOX_LAYER;
}

std::shared_ptr<Player> player;

std::shared_ptr<Model> cubeModel;

SandboxLayer::~SandboxLayer() {

}

int thingyIndex = 0;

std::shared_ptr<DynamicTextBox> dynamicTextBox;

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
	se_uniformBuffer.setProjectionMatrix(Shmingo::createProjectionMatrix(45.0f, 800, 600, 0.1f, 10000.0f));
	se_uniformBuffer.setOrthoMatrix(Shmingo::createOrthoMatrix(800, 600));

	dynamicTextBox.reset(new DynamicTextBox("Current FPS: ~fps", ivec2(2, 2), ivec2(2, 2), 1, 1));



}

void SandboxLayer::onUpdate() {

	player->update();
	world.update();
}


void SandboxLayer::KeyboardCallback(KeyPressEvent* e){

	if (e->getKey() == se_KEY_ESCAPE) {

		se_layerStack.emplaceOverlay(new PauseMenuLayer());
	}
	else if (e->getKey() == se_KEY_F3) {

		se_layerStack.emplaceOverlay(new InfoLayer());
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