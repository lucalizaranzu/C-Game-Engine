#include <sepch.h>
#include <ShmingoApp.h>

ShmingoApp::ShmingoApp() :
	window(nullptr) //Can change default aspect ratio when options is implemented

{
}

LayerStack LayerStack::instance;
MasterRenderer MasterRenderer::instance;


ShmingoApp::~ShmingoApp() {
	delete window;
}

void ShmingoApp::run() {

	init();


	while (!window->shouldDisplayClose()) {
		update();
	}
}

void ShmingoApp::init() {

	ShmingoApp::window = createWindow(800, 600); //Can change default aspect ratio when options is implemented

	glfwMakeContextCurrent(window->getGLFWwindow()); //Makes context current

	gladLoadGL();

	if (!gladLoadGL()) {
		std::cerr << "Failed to initialize OpenGL context\n";
		// Handle initialization failure
	}
	se_layerStack.init();
	se_masterRenderer.init();

	se_layerStack.emplaceLayer(new SandboxLayer);

	setGLFWkeyCallback();
	setGLFWWindowCallbacks();
	setGLFWCursorPosCallback();

}

void ShmingoApp::update() {

	window->setViewport(0.0f, 0.4f, 0.8f, 1.0f); //Contains clear color and other settings

	se_layerStack.updateLayers();

	se_masterRenderer.update();

	window->swapBuffers();
	glfwPollEvents();

}