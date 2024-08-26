#pragma once

#include <ShmingoCore.h>


class MenuButton {

public:

	MenuButton(); //Default constructor

	void onMouseEnter(); //Function to call when the mouse enters the button, can be made into a standard function object in a derived custom button class
	void onMouseExit(); //Function to call when the mouse exits the button

	void onMouseDown(); //Function to call when the mouse is pressed down on the button
	void onMouseUp(); //Function to call when the mouse is released on the button

	vec2 getPosition() { return m_position; }; //Get the position of the button
	vec2 getSize() { return m_size; }; //Get the size of the button

	bool isMouseHovered() { return m_isMouseHovered; }; //Get the flag for if the mouse is hovering over the button
	bool isMouseDown() { return m_isMouseDown; }; //Get the flag for if the button is down while over the button


protected:

	std::function<void()> m_onClickFunction; //Function to call when the button is clicked

	vec2 m_position; //Position of button top left
	vec2 m_size; //Width and height of button

	bool m_isMouseHovered; //Flag for if the mouse if hovering over the button
	bool m_isMouseDown; //Flag for if the button is down while over the button

};