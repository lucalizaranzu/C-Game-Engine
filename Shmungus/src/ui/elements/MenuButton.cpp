#include "sepch.h"
#include "MenuButton.h"

MenuButton::MenuButton(vec2 position, vec2 size, size_t textureID) : 
	m_quad(Shmingo::Quad(position,size)), textureID(textureID){
}
