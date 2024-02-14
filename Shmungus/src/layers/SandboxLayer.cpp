#include "SandboxLayer.h"
#include "LayerStack.h"

SandboxLayer::SandboxLayer() {
	type = se_SANDBOX_LAYER;
}

std::shared_ptr<ColorQuadVertexArray> vertexArray;

std::shared_ptr<Player> player;

void SandboxLayer::onAttach() {

	player = std::make_shared<Player>();

	//Creating objects
	vertexArray = std::make_shared<ColorQuadVertexArray>();

	//Create a quad
	std::array<ColorQuadVertex, 4> quad1 = createColorQuad(vec2(-0.5f, 0.5f), vec4(0.2f, 1.0f, 0.7f, 1.0f));

	vertexArray->pushVertexData(quad1.data(), 4);

}

void SandboxLayer::onUpdate() {

	se_uniformBuffer.setAsActive();
	player->update();
	se_masterRenderer.submitVertexArray(vertexArray, se_DEFAULT_SHADER);


	se_uniformBuffer.setAsActive();
	se_uniformBuffer.setProjectionMatrix(createProjectionMatrix(45.0f, 800, 600, 0.1f, 100.0f));
	se_uniformBuffer.setTestFloat(1.0f);
}

void SandboxLayer::onDetach() {

}