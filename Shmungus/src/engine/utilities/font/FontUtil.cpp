#include "sepch.h"
#include "FontUtil.h"

#include "ShmingoApp.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#define STB_IMAGE_IMPLEMENTATION

bool useMipMap = false; //Generating mipmaps is slow and annoying for debugging
#ifndef se_DEBUG
bool useMipMap = true;
#endif

void Shmingo::loadFont(std::string name) {

    std::string fontPath = "assets/fonts/" + name + ".ttf";

    FT_Library ft; // Create freetype object

    if (FT_Init_FreeType(&ft)) {
        se_error("Freetype error: could not init Freetype Library");
        return;
    }

    // Load font as face
    FT_Face face;
    if (FT_New_Face(ft, fontPath.c_str(), 0, &face)) {
        se_error("Freetype error: could not load font");
        return;
    }

    // Set size to load glyphs as (in pixels)
    FT_Set_Pixel_Sizes(face, 256, 256);

    // Disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    FT_GlyphSlot slot = face->glyph;

    se_application.setFontTextureArrayID(name, 0);

    glGenTextures(1, se_application.getFontTextureArrayIDPtr(name));
    glActiveTexture(GL_TEXTURE31);
    glBindTexture(GL_TEXTURE_2D_ARRAY, se_application.getFontTextureArrayID(name));
    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_R8, 256, 256, 128, 0, GL_RED, GL_UNSIGNED_BYTE, 0);


    for (unsigned char c = 0; c < 128; c++) {
        // Load character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }

        //ENABLE THIS IN PROD
        //FT_Render_Glyph(slot, FT_RENDER_MODE_SDF);

        glTexSubImage3D(
            GL_TEXTURE_2D_ARRAY,
            0, 0, 0, int(c),
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows, 1,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );

        if (useMipMap) {
            glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
            glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        }
        else {
            glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }

        // Set texture options
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        // Now store character for later use
        Shmingo::Character character = {
            GLuint(c),
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<GLuint>(face->glyph->advance.x >> 6) // Convert 1/64th pixels to pixels
        };
        se_application.declareCharacterFontInfo(name, c, character);
    }
    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

    // Destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

void Shmingo::bindFontTextureToShader(std::shared_ptr<ShaderProgram> shader, std::string fontName) {

    glActiveTexture(GL_TEXTURE31);
    glBindTexture(GL_TEXTURE_2D_ARRAY, se_application.getFontTextureArrayID(fontName));

    GLint location = glGetUniformLocation(shader->getProgramID(), "font"); // Get uniform location
    glUniform1i(location, 31); // Set uniform value to 0

}
