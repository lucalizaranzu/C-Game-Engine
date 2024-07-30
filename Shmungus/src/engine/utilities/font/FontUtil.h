#pragma once

#include <ShmingoCore.h>
#include "ShaderProgram.h"

namespace Shmingo {

    //Represents a character of a specific font
    struct Character {
        int TextureID; // ID handle of the glyph texture
        glm::ivec2   Size;      // Size of glyph
        glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
        unsigned int Advance;   // Horizontal offset to advance to next glyph
    };

	void loadFont(std::string name);


    void bindFontTextureToShader(std::shared_ptr<ShaderProgram> shader, std::string fontName);

}