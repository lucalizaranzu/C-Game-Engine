#pragma once
#include <ShmingoCore.h>
#include "Vertex.h"

std::array<ColorQuadVertex, 4> createColorQuad(vec2 position, vec4 color);
std::array<ColorQuadVertex, 24> createColorCube(vec2 position, vec4 color);