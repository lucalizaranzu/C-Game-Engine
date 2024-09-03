#pragma once


class Event {

public:

	inline void setHandled() { handled = true; }; //Prevents propogation to other layers
	inline bool isHandled() { return handled; };

protected:

	bool handled = false; //Set to false by default
};


class TestEvent : public Event{

public:

	TestEvent(int x, char y) : m_x(x), m_y(y) {};
	int m_x;
	char m_y;

};

//Keyboard events --------------------------------------------------------------------------------------------

class KeyboardEvent : public Event {

public:

	KeyboardEvent(int key, int scancode, int action, int mods) : key(key), scancode(scancode), action(action), mods(mods) {};

	int getKey() { return key; };
	int getScancode() { return scancode; };
	int getAction() { return action; };
	int getMods() { return mods; };


protected:

	int key;
	int scancode;
	int action;
	int mods;

};


class KeyPressEvent : public KeyboardEvent {

public:

	KeyPressEvent(int key, int scancode, int action, int mods) : KeyboardEvent(key, scancode, action, mods) {}
};

class KeyReleaseEvent : public KeyboardEvent {

public:

	KeyReleaseEvent(int key, int scancode, int action, int mods) : KeyboardEvent(key, scancode, action, mods) {}
};


class KeyRepeatEvent : public KeyboardEvent {

public:

	KeyRepeatEvent(int key, int scancode, int action, int mods) : KeyboardEvent(key, scancode, action, mods) {}
};

//Mouse events --------------------------------------------------------------------------------------------

class MouseEvent : public Event {

public:

	MouseEvent(int button, int action, int mods) : button(button), action(action), mods(mods) {};

	int getButton() { return button; };
	int getAction() { return action; };
	int getMods() { return mods; };

protected:

	int button;
	int action;
	int mods;

};


class MouseClickEvent : public MouseEvent {

public:

	MouseClickEvent(int button, int action, int mods) : MouseEvent(button,action,mods){}

};

class MouseReleaseEvent : public MouseEvent {

public:

	MouseReleaseEvent(int button, int action, int mods) : MouseEvent(button, action, mods) {}

};


class MouseDragEvent : public Event{

public:
	
	MouseDragEvent(float xDiff, float yDiff, float xPosition, float yPosition) : xOffset(xDiff), yOffset(yDiff), xPos(xPosition), yPos(yPosition) {}

	float getXOffset() { return xOffset; };
	float getYOffset() { return yOffset; };

	float getXPos() { return xPos; };
	float getYPos() { return yPos; };

protected:

	float xOffset;
	float yOffset;

	float xPos;
	float yPos;

};

//Window events --------------------------------------------------------------------------------------------

class WindowEvent : public Event {
	//Nothing for now
};


class WindowResizeEvent : public WindowEvent {

public:

	WindowResizeEvent(float oldWidth, float oldHeight, float newWidth, float newHeight) : oldWidth(oldWidth), oldHeight(oldHeight), newWidth(newWidth), newHeight(newHeight) {};

	float getOldWidth() { return oldWidth; };
	float getOldHeight() { return oldHeight; };
	float getNewWidth() { return newWidth; };
	float getNewHeight() { return newHeight; };

protected:

	float oldWidth;
	float oldHeight;
	float newWidth;
	float newHeight;

};

