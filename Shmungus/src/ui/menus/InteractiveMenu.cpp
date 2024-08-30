#include <sepch.h>

#include "InteractiveMenu.h"
#include "MasterRenderer.h"


InteractiveMenu::InteractiveMenu(){

	Shmingo::TextureAtlas textureAtlas = Shmingo::TextureAtlas(1250, 1250, true);

	textureAtlas.addTexture(0, "menu_elements/purple.png");
	textureAtlas.addTexture(1, "menu_elements/blue.png");
	textureAtlas.addTexture(2, "menu_elements/green.png");
	textureAtlas.addTexture(3, "menu_elements/red.png");


	m_vertexArray.reset(new TexturedQuadVertexArrayAtlas(textureAtlas)); //Initialize vertex array
}

void InteractiveMenu::init(){
	m_vertexArray->init();

}

void InteractiveMenu::update(){
	se_masterRenderer.submitInstancedVertexArray(m_vertexArray, se_MENU_SHADER);
}

void InteractiveMenu::submitButton(MenuButton button){
	se_log("Texture ID: " + std::to_string(button.getTextureID()));
	m_vertexArray->submitQuad(button.getQuad(), button.getTextureID());
}

