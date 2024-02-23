#include <sepch.h>

#include "Layer.h"
#include "LayerStack.h"

#include "Player.h"
#include "MiscTools.h"
#include "TextureTools.h"
#include "ModelTools.h"

SandboxLayer::SandboxLayer() {
	type = se_SANDBOX_LAYER;
}

std::shared_ptr<EntityVertexArray> vertexArray;

std::shared_ptr<Player> player;
std::shared_ptr<Entity> entity;

SandboxLayer::~SandboxLayer() {

}

void SandboxLayer::onAttach() {

	se_layerStack.addListener<SandboxLayer, KeyPressEvent>(se_SANDBOX_LAYER, this, &SandboxLayer::KeyboardCallback);

	//Init functions
	disableGLFWCursor();

	//Creating objects
	player = std::make_shared<Player>();
	vertexArray = std::make_shared<EntityVertexArray>();

	//Create an entity

	Model cubeModel = createCubeModel("funnyimage.png");
	entity = std::make_shared<Entity>(cubeModel);

	vertexArray->pushVertexData(entity);

	se_uniformBuffer.setAsActive();
	se_uniformBuffer.setProjectionMatrix(createProjectionMatrix(45.0f, 800, 600, 0.1f, 100.0f));
}

void SandboxLayer::onUpdate() {

	player->update();
	se_masterRenderer.submitVertexArray(vertexArray, se_DEFAULT_SHADER);

}

void SandboxLayer::KeyboardCallback(KeyPressEvent* e){

	if (e->getKey() == se_KEY_ESCAPE) {

		se_layerStack.emplaceOverlay(new PauseMenuLayer());
	}
	e->setHandled();
}

void SandboxLayer::onDetach() {

}
