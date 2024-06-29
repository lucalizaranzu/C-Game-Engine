#pragma once

#include <ShmingoCore.h>

namespace Shmingo {

    //Represents a character of a specific font
    struct Character {
        int TextureID; // ID handle of the glyph texture
        glm::ivec2   Size;      // Size of glyph
        glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
        unsigned int Advance;   // Horizontal offset to advance to next glyph
    };

	void loadFont(std::string name);




}