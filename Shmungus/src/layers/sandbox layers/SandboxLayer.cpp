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
#include "ChunkVertexArray.h"
#include "ShmingoApp.h"



SandboxLayer::SandboxLayer() : world(World()) {
	type = Shmingo::SANDBOX_LAYER;
}

std::shared_ptr<Player> player;

std::shared_ptr<Model> cubeModel;

std::shared_ptr<ChunkVertexArray> chunkVertexArray;

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
	se_uniformBuffer.setProjectionMatrix(Shmingo::createProjectionMatrix(45.0f, se_application.getWindow()->getWidth(), se_application.getWindow()->getHeight(), 0.1f, 10000.0f));
	se_uniformBuffer.setOrthoMatrix(Shmingo::createOrthoMatrix(se_application.getWindow()->getWidth(), se_application.getWindow()->getHeight()));




	//Debug stuff
	chunkVertexArray.reset(new ChunkVertexArray());
	chunkVertexArray->init();


	size_t trianglesToDraw = 12;
	/*
	for (size_t i = 0; i < trianglesToDraw; i++) {
		chunkVertexArray->submitTriangleDebug(i, uvec3(0, 0, 0), 0, i + 12);
	}

	for (size_t i = 0; i < 12; i++) {
		chunkVertexArray->submitTriangleDebug(i + trianglesToDraw, uvec3(2, 2, 0), 0, i);
	}
	*/
	chunkVertexArray->submitTriangleDebug(0, uvec3(0, 0, 0), 0, 32);
	chunkVertexArray->submitTriangleDebug(1, uvec3(0, 0, 0), 0, 19);
	chunkVertexArray->submitTriangleDebug(2, uvec3(0, 0, 0), 0, 3);
	chunkVertexArray->submitTriangleDebug(3, uvec3(0, 0, 0), 0, 33);



}

void SandboxLayer::onUpdate() {

	player->update();
	se_masterRenderer.submitTerrainVertexArray(chunkVertexArray, se_TERRAIN_SHADER);
	world.update();

	std::string thing = se_application.getApplicationInfo("entityCount");
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