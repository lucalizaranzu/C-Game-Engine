#include <sepch.h>
#include <ShmingoApp.h>
#include <MiscTools.h>


double lastFrameTime = 0.0f;
World* dummyWorld;

int totalFrames = 0;

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

void Shmingo::ShmingoApp::recalculateTextSpacing(float oldDisplayWidth, float oldDisplayHeight, float newDisplayWidth, float newDisplayHeight){
	for (auto& infoSpace : infoSpaces) {
		infoSpace->recalculateTextSpacing(oldDisplayWidth, oldDisplayHeight, newDisplayWidth, newDisplayHeight); //Recalculates text spacing of all info spaces
	}
}


void Shmingo::ShmingoApp::run() {

	init();


	while (!window->shouldDisplayClose()) {
		update();
	}

	se_layerStack.cleanUp();
}



void Shmingo::ShmingoApp::init() {
	ShmingoApp::window = Shmingo::createWindow(1920, 1080); //Can change default aspect ratio when options is implemented

	glfwMakeContextCurrent(window->getGLFWwindow()); //Makes context current

	gladLoadGL();

	if (!gladLoadGL()) {
		std::cerr << "Failed to initialize OpenGL context\n";
		// Handle initialization failure
	}

	//Set up monitor aspect ratio as a global variable
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	setApplicationInfo(Shmingo::PRIMARY_MONITOR_WIDTH, "primary_monitor_width", std::to_string(mode->width));
	setApplicationInfo(Shmingo::PRIMARY_MONITOR_HEIGHT, "primary_monitor_height", std::to_string(mode->height));

	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	initGlobalVariables(); //Initialize global variables after GLAD is loaded

	setApplicationInfo(Shmingo::FPS, "fps", std::to_string(2));

	se_layerStack.init();

	Shmingo::initModels();
	Shmingo::declareTypeCorrespondence();
	Shmingo::loadFont("arial");
	Shmingo::loadFont("Minecraft");

	se_masterRenderer.init();

	se_layerStack.emplaceLayer(new SandboxLayer);

	glfwSwapInterval(0); //Disable v sync

	Shmingo::setGLFWkeyCallback();
	Shmingo::setGLFWMouseButtonCallback();
	Shmingo::setGLFWWindowCallbacks();
	Shmingo::setGLFWCursorPosCallback();

	se_uniformBuffer.setOrthoMatrix(Shmingo::createOrthoMatrix((float)window->getWidth(), (float)window->getHeight())); //Initializes orthographic projection matrix

}

void Shmingo::ShmingoApp::update() {

	window->setViewport(0.0f, 0.4f, 0.8f, 1.0f); //Contains clear color and other settings

	double time = glfwGetTime(); //TODO Make platform specific later on - similar to gl functions
	deltaTime = time - lastFrameTime;

	lastFrameTime = time;
	timeElapsed += deltaTime;

	if (Shmingo::isTimeMultipleOf(0.2)) {
		setApplicationInfo(Shmingo::FPS, "fps", std::to_string(totalFrames * 5).substr(0, 4)); //First four digits of FPS
		totalFrames = 0;
	}

	se_layerStack.updateLayers();

	se_masterRenderer.update();
	se_uniformBuffer.setElapsedTime((float)timeElapsed);


	window->swapBuffers();
	glfwPollEvents();

	totalFrames++;

}




void Shmingo::ShmingoApp::initGlobalVariables(){
	glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &minimumUniformBlockOffset);
}

void Shmingo::ShmingoApp::setApplicationInfo(Shmingo::ApplicationInfoKey key, std::string stringKey, std::string value){

	applicationInfoStringToKeyMap[stringKey] = key;
	applicationInfo[key] = value;
}

uint8_t Shmingo::ShmingoApp::getTextColor(char c){

	if (c >= '0' && c <= '9') {
		return c - '0';
	}
	else if (c >= 'a' && c <= 'z') {
		return c - 'a' + 10;
	}
	else if (c >= 'A' && c <= 'Z') {
		return c - 'A' + 36;
	}
	else {
		// Return an invalid value if the character is not in the expected range
		return 0x3F; //Default to 0
	}
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
