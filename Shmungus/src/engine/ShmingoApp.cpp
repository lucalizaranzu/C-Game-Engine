#include <sepch.h>
#include <ShmingoApp.h>


double lastFrameTime = 0.0f;
World* dummyWorld;
Shmingo::ShmingoApp::ShmingoApp() :
	window(nullptr), //Can change default aspect ratio when options is implemented,
	currentWorld(dummyWorld)
{
}

LayerStack LayerStack::instance;
MasterRenderer MasterRenderer::instance;


Shmingo::ShmingoApp::~ShmingoApp() {
	delete window;
}


void Shmingo::ShmingoApp::run() {

	init();


	while (!window->shouldDisplayClose()) {
		update();
	}

	se_layerStack.cleanUp();
}



void Shmingo::ShmingoApp::init() {
	ShmingoApp::window = Shmingo::createWindow(800, 600); //Can change default aspect ratio when options is implemented

	glfwMakeContextCurrent(window->getGLFWwindow()); //Makes context current

	gladLoadGL();

	if (!gladLoadGL()) {
		std::cerr << "Failed to initialize OpenGL context\n";
		// Handle initialization failure
	}

	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	initGlobalVariables(); //Initialize global variables after GLAD is loaded

	se_layerStack.init();

	Shmingo::initModels();
	Shmingo::declareTypeCorrespondence();
	Shmingo::loadFont("arial");

	se_masterRenderer.init();

	se_layerStack.emplaceLayer(new SandboxLayer);

	setGLFWkeyCallback();
	setGLFWWindowCallbacks();
	setGLFWCursorPosCallback();

}

void Shmingo::ShmingoApp::update() {

	window->setViewport(0.0f, 0.4f, 0.8f, 1.0f); //Contains clear color and other settings

	double time = glfwGetTime(); //TODO Make platform specific later on - similar to gl functions
	deltaTime = time - lastFrameTime;

	lastFrameTime = time;
	timeElapsed += deltaTime;

	setApplicationInfo("FPS", std::to_string(1.0f / deltaTime));



	se_layerStack.updateLayers();

	se_masterRenderer.update();
	se_uniformBuffer.setElapsedTime((float)timeElapsed);

	window->swapBuffers();
	glfwPollEvents();

}




void Shmingo::ShmingoApp::initGlobalVariables(){
	glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &minimumUniformBlockOffset);
}

void Shmingo::ShmingoApp::setApplicationInfo(std::string key, std::string value){

	applicationInfo[key] = value;
}

void Shmingo::ShmingoApp::setCurrentWorld(World* world) {
	currentWorld = world;
}

void Shmingo::ShmingoApp::declareEntityType(std::type_index typeIndex, EntityType type) {
	entityTypes.push_back(type);
}

void Shmingo::ShmingoApp::setFontTextureArrayID(std::string font, GLuint newID){
	fontTextureArrayMap[font] = newID;
}
