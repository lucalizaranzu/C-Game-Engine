#include "VertexTools.h"

std::array<ColorQuadVertex, 4> createColorQuad(vec2 position, vec4 color) {

	ColorQuadVertex v1 = { position.x,position.y,color.x,color.y,color.z,color.w};
	ColorQuadVertex v2 = { position.x,position.y - 1.0f,color.x,color.y,color.z,color.w };
	ColorQuadVertex v3 = { position.x + 1.0f,position.y - 1.0f,color.x,color.y,color.z,color.w };
	ColorQuadVertex v4 = { position.x + 1.0f,position.y,color.x,color.y,color.z,color.w };

	return std::array<ColorQuadVertex, 4>{v1, v2, v3, v4};
}