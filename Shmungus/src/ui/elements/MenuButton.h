#pragma once

#include <ShmingoCore.h>
#include "TextBox.h"

class MenuButton {

public:

	MenuButton(vec2 position, vec2 size, size_t textureID, std::string text, GLuint fontSize, GLuint lineSpacing); //Default constructor

	void checkMouseHovered(vec2 mousePosition); //Check if the mouse is hovering over the button
	void setIsMouseDown(bool isMouseDown) { m_isMouseDown = isMouseDown; }; //Set the flag for if the button is down while over the button
	void setIsMouseHovered(bool isMouseHovered) { m_isMouseHovered = isMouseHovered; }; //Set the flag for if the mouse is hovering over the button

	std::function<void()> getOnClickFunction() { return m_onClickFunction; }; //Get the function to call when the button is clicked
	std::function<void()> getOnMouseEnterFunction() { return m_onMouseEnterFunction; }; //Get the function to call when the mouse enters the button
	std::function<void()> getOnMouseExitFunction() { return m_onMouseExitFunction; }; //Get the function to call when the mouse exits the button

	void setOnClickFunction(std::function<void()> onClickFunction) { m_onClickFunction = onClickFunction; }; //Set the function to call when the button is clicked
	void setOnMouseEnterFunction(std::function<void()> onMouseEnterFunction) { m_onMouseEnterFunction = onMouseEnterFunction; }; //Set the function to call when the mouse enters the button
	void setOnMouseExitFunction(std::function<void()> onMouseExitFunction) { m_onMouseExitFunction = onMouseExitFunction; }; //Set the function to call when the mouse exits the button

	void setVaoID(size_t vaoID) { m_vaoID = vaoID; }; //Set the vertex array ID of the button

	vec2 getPosition() { return m_quad.position; }; //Get the position of the button
	vec2 getSize() { return m_quad.size; }; //Get the size of the button

	inline void setTextureID(size_t newTextureID) { textureID = newTextureID; }; //Set the texture ID of the button
	size_t getTextureID() { return textureID; }; //Get the texture ID of the button
	size_t getVaoID() { return m_vaoID; }; //Get the vertex array ID of the button

	Shmingo::Quad getQuad() { return m_quad; }; //Get the quad of the button

	bool isMouseHovered() { return m_isMouseHovered; }; //Get the flag for if the mouse is hovering over the button
	bool isMouseDown() { return m_isMouseDown; }; //Get the flag for if the button is down while over the button

	TextBox& getTextBox() { return m_textBox; }; //Get the text box of the button


protected:

	size_t m_vaoID = 0; //Index in the vertex array

	std::function<void()> m_onClickFunction; //Function to call when the button is clicked
	std::function<void()> m_onMouseEnterFunction; //Function to call when the mouse enters the button
	std::function<void()> m_onMouseExitFunction; //Function to call when the mouse exits the button



	Shmingo::Quad m_quad; //Quad to represent the button

	TextBox m_textBox; //TextBox to represent the text on the button

	bool m_isMouseHovered = false; //Flag for if the mouse if hovering over the button
	bool m_isMouseDown = false; //Flag for if the button is down while over the button

	size_t textureID; //Texture ID for the button

};