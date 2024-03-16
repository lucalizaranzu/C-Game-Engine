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

std::shared_ptr<InstancedVertexArray<DefaultEntity>> defaultInstancedVertexArray;

std::shared_ptr<Player> player;

std::shared_ptr<Entity> regularEntity;

std::shared_ptr<Model> cubeModel;

SandboxLayer::~SandboxLayer() {

}

void SandboxLayer::onAttach() {


	Texture2D funnyTexture = createTexture2D("ramranch.png");

	se_layerStack.addListener<SandboxLayer, KeyPressEvent>(se_SANDBOX_LAYER, this, &SandboxLayer::KeyboardCallback);

	//Init functions
	disableGLFWCursor();

	//Creating other objects
	cubeModel.reset(createModelPointer(std::bind(createCubeModel, vec3(0.0f, 0.0f, 0.0f), funnyTexture)));

	defaultInstancedVertexArray = std::make_shared<InstancedVertexArray<DefaultEntity>>(cubeModel);
	defaultInstancedVertexArray->linkTexture(funnyTexture, 0);

	//Create an entity

	std::shared_ptr<DefaultEntity> entity = std::make_shared<DefaultEntity>(vec3(3.0f,2.0f,-1.0f),vec3(0.0f,0.0f,0.0f));
	std::shared_ptr<DefaultEntity>entity2 = std::make_shared<DefaultEntity>(vec3(1.0f,0.0f,-1.0f),vec3(0.0f,0.0f,0.0f));
	std::shared_ptr<DefaultEntity>entity3 = std::make_shared<DefaultEntity>(vec3(-2.0f,-1.0f,-2.0f),vec3(0.0f,0.0f,0.0f));

	player.reset(new Player(createCubeModel(vec3(0.0f,0.0f,0.0f),funnyTexture)));

	defaultInstancedVertexArray->submitInstanceData(entity);
	defaultInstancedVertexArray->submitInstanceData(entity2);
	defaultInstancedVertexArray->submitInstanceData(entity3);

	se_uniformBuffer.setAsActive();
	se_uniformBuffer.setProjectionMatrix(createProjectionMatrix(45.0f, 800, 600, 0.1f, 10000.0f));

}

void SandboxLayer::onUpdate() {

	player->update();

	se_masterRenderer.submitInstancedVertexArray(defaultInstancedVertexArray);
}

void SandboxLayer::KeyboardCallback(KeyPressEvent* e){

	if (e->getKey() == se_KEY_ESCAPE) {

		se_layerStack.emplaceOverlay(new PauseMenuLayer());
	}
	e->setHandled();
}

void SandboxLayer::onDetach() {

}