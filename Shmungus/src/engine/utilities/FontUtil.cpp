#include "sepch.h"
#include "FontUtil.h"

#include "ShmingoApp.h"

#include <ft2build.h>
#include FT_FREETYPE_H

void Shmingo::loadFont(std::string name) {

    std::string fontPath = "assets/fonts/" + name + ".ttf";

    FT_Library ft; //Create freetype object

    if (FT_Init_FreeType(&ft)) {
        se_error("Freetype error: could not init Freetype Library");
        return;
    }

    // load font as face
    FT_Face face;
    if (FT_New_Face(ft, fontPath.c_str(), 0, &face)) {
        se_error("Freetype error: could not load font");
        return;
    }

    // set size to load glyphs as
    FT_Set_Pixel_Sizes(face, 256, 256);

    // disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    se_application.setFontTextureArrayID(name, 0);
    glGenTextures(1, se_application.getFontTextureArrayIDPtr(name));
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, se_application.getFontTextureArrayID(name));
    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_R8, 256, 256, 128, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

    for (unsigned char c = 0; c < 128; c++) {
        // Load character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        glTexSubImage3D(
            GL_TEXTURE_2D_ARRAY,
            0, 0, 0, int(c),
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows, 1,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        // set texture options
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // now store character for later use
        Shmingo::Character character = {
            GLuint(c),
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<GLuint>(face->glyph->advance.x)
        };
        se_application.declareCharacterFontInfo(c, character);
    }
    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

    // destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}