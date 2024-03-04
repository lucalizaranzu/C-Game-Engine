#pragma once

#include <ShmingoCore.h>

#include "EventBus.h"
#include "VertexArray.h"
#include "Vertex.h"

enum LayerType {

	se_SANDBOX_LAYER,
	se_PAUSEMENU_LAYER

};

class LayerStack;

class Layer {

	friend class LayerStack;

public:

	Layer(); //Takes in the layer stack it will be a part of
	virtual ~Layer();


	template<class T, class EventType>
	void addListener(T* instance, void (T::* instanceFunction)(EventType*)) {

		eventBus.subscribe(instance, instanceFunction); //Adds function to this layer's event bus
	}

protected:

	virtual void onAttach() = 0;
	virtual void onDetach() = 0;
	virtual void onUpdate() = 0;

	EventBus eventBus;

	LayerType type;
};


class SandboxLayer : public Layer {

public:

	SandboxLayer();
	~SandboxLayer();

	static std::shared_ptr<Texture2D> getFunnyTexture();

private:

	void onAttach() override;
	void onDetach() override;
	void onUpdate() override;

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

