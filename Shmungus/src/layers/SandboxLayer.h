#pragma once

#include <ShmingoCore.h>
#include <Engine.h>

#include "Layer.h"
#include "Entity.h"
#include "Player.h"

#include "UniformBuffer.h"
#include "Matrices.h"

class SandboxLayer : public Layer {

public:

	SandboxLayer();

private:

	void onAttach() override;
	void onDetach() override;
	void onUpdate() override;

};