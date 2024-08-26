#include <sepch.h>

#include "InteractiveMenu.h"
#include "MasterRenderer.h"


InteractiveMenu::InteractiveMenu(){

	Shmingo::TextureAtlas textureAtlas = Shmingo::TextureAtlas(1250, 1250, true);

	m_vertexArray.reset(new TexturedQuadVertexArrayAtlas(textureAtlas)); //Initialize vertex array
}

void InteractiveMenu::init(){
	m_vertexArray->init();

	m_vertexArray->declareTexture(0, "menu_elements/blue");
	m_vertexArray->declareTexture(1, "menu_elements/red");
	m_vertexArray->declareTexture(2, "menu_elements/green");
	m_vertexArray->declareTexture(3, "menu_elements/purple");
}

void InteractiveMenu::update(){
	se_masterRenderer.submitInstancedVertexArray(m_vertexArray, se_MENU_SHADER);
}

