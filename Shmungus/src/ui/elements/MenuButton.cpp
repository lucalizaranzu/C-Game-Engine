#include "sepch.h"
#include "MenuButton.h"

MenuButton::MenuButton(vec2 position, vec2 size, size_t textureID, std::string text, GLuint fontSize, GLuint lineSpacing) :
	m_quad(Shmingo::Quad(vec2(position.x, -1 * position.y), size)), textureID(textureID),
	m_textBox(TextBox(text, vec2(position.x, position.y + (0.06f / fontSize)), size, fontSize, lineSpacing , Shmingo::CENTER)) {
}

void MenuButton::checkMouseHovered(vec2 mousePosition){


	if (mousePosition.x >= m_quad.position.x && mousePosition.x <= m_quad.position.x + m_quad.size.x &&
		mousePosition.y >= -1 * m_quad.position.y && mousePosition.y <= (-1 * m_quad.position.y + m_quad.size.y)) {
		if (m_isMouseHovered == false) {
			m_onMouseEnterFunction(); //Call the mouse enter function
			m_isMouseHovered = true; //Set to true
		}
	}
	else {
		if (m_isMouseHovered == true) {
			m_onMouseExitFunction(); //Call the mouse exit function
			m_isMouseHovered = false; //Set to false
		}
	}
}
