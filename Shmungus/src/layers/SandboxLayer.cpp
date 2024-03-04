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
std::shared_ptr<ColorQuadVertexArray> colorVertexArray;

std::shared_ptr<Player> player;

std::shared_ptr<Entity> entity;
std::shared_ptr<Entity> entity2;
std::shared_ptr<Entity> entity3;
std::shared_ptr<Entity> entity4;

std::shared_ptr<Texture2D> whiteTexture;
std::shared_ptr<Texture2D> funnyTexture;
std::shared_ptr<Texture2D> ramRanch;
std::shared_ptr<Texture2D> redTexture;

SandboxLayer::~SandboxLayer() {

}

void SandboxLayer::onAttach() {

	se_layerStack.addListener<SandboxLayer, KeyPressEvent>(se_SANDBOX_LAYER, this, &SandboxLayer::KeyboardCallback);

	//Init functions
	disableGLFWCursor();

	//Creating engine objects
	vertexArray = std::make_shared<EntityVertexArray>();
	colorVertexArray = std::make_shared<ColorQuadVertexArray>();
	std::array<ColorQuadVertex, 4> colorQuad = createColorQuad(vec2(0.0f, 0.0f), vec4(0.1f, 0.8f, 0.5f, 1.0f));

	//Creating other objects

	funnyTexture = createTexture2D("funnyimage.png");
	whiteTexture = createTexture2D("white.png");
	ramRanch = createTexture2D("ramranch.png");
	redTexture = createTexture2D("red.png");

	vertexArray->declareTextureSlot(funnyTexture, 0);
	vertexArray->declareTextureSlot(ramRanch, 1);
	vertexArray->declareTextureSlot(whiteTexture, 2);
	vertexArray->declareTextureSlot(redTexture, 3);

	//Create an entity

	Model cubeModel = createQuadModel(vec3(- 1.0f, 1.0f, -1.0f), vertexArray->getTextureID(funnyTexture));
	Model cubeModel2 = createQuadModel(vec3(2.0f, 1.0f, -1.0f), vertexArray->getTextureID(ramRanch));
	Model cubeModel3 = createQuadModel(vec3(5.0f, 1.0f, -1.0f), vertexArray->getTextureID(whiteTexture));
	Model cubeModel4 = createQuadModel(vec3(8.0f, 1.0f, -1.0f), vertexArray->getTextureID(redTexture));


	entity = std::make_shared<Entity>(cubeModel);
	entity2 = std::make_shared<Entity>(cubeModel2);
	entity3 = std::make_shared<Entity>(cubeModel3);
	entity4 = std::make_shared<Entity>(cubeModel4);

	player = std::make_shared<Player>(cubeModel);

	vertexArray->pushVertexData(entity);
	vertexArray->pushVertexData(entity2);
	vertexArray->pushVertexData(entity3);
	vertexArray->pushVertexData(entity4);

	//vertexArray->removeVertexData(entity2);
	colorVertexArray->pushVertexData(colorQuad.data(), 4);

	se_uniformBuffer.setAsActive();
	se_uniformBuffer.setProjectionMatrix(createProjectionMatrix(45.0f, 800, 600, 0.1f, 1000.0f));

}

void SandboxLayer::onUpdate() {

	player->update();

	se_masterRenderer.submitVertexArray(vertexArray, se_ENTITY_SHADER);
	//se_masterRenderer.submitVertexArray(colorVertexArray, se_DEFAULT_SHADER);

}

void SandboxLayer::KeyboardCallback(KeyPressEvent* e){

	if (e->getKey() == se_KEY_ESCAPE) {

		se_layerStack.emplaceOverlay(new PauseMenuLayer());
	}
	e->setHandled();
}

void SandboxLayer::onDetach() {

}

std::shared_ptr<Texture2D> SandboxLayer::getFunnyTexture() {
	return funnyTexture;
}