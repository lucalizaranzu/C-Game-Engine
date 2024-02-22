#include <sepch.h>

#include "Layer.h"
#include "LayerStack.h"

#include "Player.h"
#include "MiscTools.h"

SandboxLayer::SandboxLayer() {
	type = se_SANDBOX_LAYER;
}

std::shared_ptr<ColorQuadVertexArray> vertexArray;

std::shared_ptr<Player> player;

SandboxLayer::~SandboxLayer() {

}

void SandboxLayer::onAttach() {

	se_layerStack.addListener<SandboxLayer, KeyPressEvent>(se_SANDBOX_LAYER, this, &SandboxLayer::KeyboardCallback);

	//Init functions
	disableGLFWCursor();

	//Creating objects
	player = std::make_shared<Player>();
	vertexArray = std::make_shared<ColorQuadVertexArray>();

	//Create a quad
	std::array<ColorQuadVertex, 4> quad1 = createColorQuad(vec2(-0.5f, 0.5f), vec4(0.2f, 1.0f, 0.7f, 1.0f));

	vertexArray->pushVertexData(quad1.data(), 4);

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
