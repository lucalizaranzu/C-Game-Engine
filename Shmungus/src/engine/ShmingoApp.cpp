#include <sepch.h>
#include <ShmingoApp.h>

World tempWorld;

Shmingo::ShmingoApp::ShmingoApp() :
	window(nullptr), //Can change default aspect ratio when options is implemented,
	currentWorld(tempWorld)
{
}

LayerStack LayerStack::instance;
MasterRenderer MasterRenderer::instance;


Shmingo::ShmingoApp::~ShmingoApp() {
	delete window;
}

void Shmingo::ShmingoApp::declareEntiyType(EntityType type){
	entityTypes.push_back(type);
}

void Shmingo::ShmingoApp::run() {

	init();


	while (!window->shouldDisplayClose()) {
		update();
	}
}

void Shmingo::ShmingoApp::init() {
	ShmingoApp::window = Shmingo::createWindow(800, 600); //Can change default aspect ratio when options is implemented

	glfwMakeContextCurrent(window->getGLFWwindow()); //Makes context current

	gladLoadGL();

	if (!gladLoadGL()) {
		std::cerr << "Failed to initialize OpenGL context\n";
		// Handle initialization failure
	}
	se_layerStack.init();
	Shmingo::initModels();
	se_masterRenderer.init();

	se_layerStack.emplaceLayer(new SandboxLayer);

	setGLFWkeyCallback();
	setGLFWWindowCallbacks();
	setGLFWCursorPosCallback();

}

void Shmingo::ShmingoApp::update() {

	window->setViewport(0.0f, 0.4f, 0.8f, 1.0f); //Contains clear color and other settings

	se_layerStack.updateLayers();

	se_masterRenderer.update();

	window->swapBuffers();
	glfwPollEvents();

}