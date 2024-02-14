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
