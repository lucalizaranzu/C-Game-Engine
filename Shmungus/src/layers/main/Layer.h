#pragma once

#include <ShmingoCore.h>

#include "EventBus.h"
#include "InstancedVertexArray.h"
#include "World.h"
#include "InfoSpace.h"
#include "InteractiveMenu.h"

class LayerStack;

class Layer {

	friend class LayerStack;

public:

	Layer(); //Takes in the layer stack it will be a part of
	//virtual ~Layer();

	virtual void cleanUp();

	template<class T, class EventType>
	void addListener(T* instance, void (T::* instanceFunction)(EventType*)) {

		eventBus.subscribe(instance, instanceFunction); //Adds function to this layer's event bus
	}

protected:

	virtual void onAttach() = 0;
	virtual void onDetach() = 0;
	virtual void onUpdate() = 0;

	EventBus eventBus;

	Shmingo::LayerType type = Shmingo::SANDBOX_LAYER;
};



class SandboxLayer : public Layer {

public:

	SandboxLayer();
	~SandboxLayer();

protected:

	World world;

	void onAttach() override;
	void onDetach() override;
	void onUpdate() override;

	void cleanUp() override;

private:

	void KeyboardCallback(KeyPressEvent* e);

};




class PauseMenuLayer : public Layer {

public:

	PauseMenuLayer();

private:

	void onAttach() override;
	void onDetach() override;
	void onUpdate() override;

	void keybordCallback(KeyPressEvent* e);
	void mouseDragCallback(MouseDragEvent* e);

};


class InfoLayer : public Layer {

public:

	InfoLayer();

protected:

	void onAttach() override;
	void onDetach() override;
	void onUpdate() override;

	void keybordCallback(KeyPressEvent* e);


	InfoSpace infoSpace;

};


class TopLayer : public Layer {

public:
	TopLayer();

protected:

	void onAttach() override;
	void onDetach() override;
	void onUpdate() override;

	void mouseClickCallback(MouseClickEvent* e);
	void mouseReleaseCallback(MouseReleaseEvent* e);


};