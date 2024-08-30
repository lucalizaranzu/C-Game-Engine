#include <sepch.h>

#include "TextureAtlas.h"
#define STB_IMAGE_IMPLEMENTATION

Shmingo::TextureAtlas::TextureAtlas(size_t width, size_t height, bool uniformTextureDimensions) 
	: texture(Texture2D(width, height)), m_UniformTextureDimensions(uniformTextureDimensions), m_Width(width), m_Height(height){

}

void Shmingo::TextureAtlas::addTexture(size_t textureID, std::string filePath){
	std::string realFilePath = "assets/textures/" + filePath;


	int int_width, int_height, channels;

	stbi_set_flip_vertically_on_load(1); //Flip texture to offset OpenGL flipping
	stbi_uc* textureData = stbi_load(realFilePath.c_str(), &int_width, &int_height, &channels, 0); //stbi_uc is a typdef of unsigned char*, can use to initialize texture

	if (!textureData) {
		std::string errorMsg = "Failed to add texture to atlas: ";
		errorMsg += filePath;
		se_error(errorMsg);
		throw std::runtime_error(errorMsg);
	}

	float width = (float)int_width / m_Width; //Width in percentage of atlas
	float height = (float)int_height / m_Height; //Height in percentage of atlas


	if (m_UniformTextureDimensions) {

		float leftX = 0, bottomY = 0;

		if (texturesInAtlas > 0) {

			Shmingo::QuadTextureCoords currentLastCoords = m_textureCoords[texturesInAtlas - 1];

			if (currentLastCoords.topRight.x + width <= 1) {

				leftX = currentLastCoords.topRight.x;
				bottomY = currentLastCoords.bottomRight.y;

			}

			//Overflow to next section down

			else {
				if (currentLastCoords.topLeft.y + height <= 1) {
					leftX = 0;
					bottomY = currentLastCoords.bottomLeft.y + height;

				}
				else {
					se_log("Atlas is full");
					return;
				}
			}

		}
		
		Shmingo::QuadTextureCoords coords = Shmingo::QuadTextureCoords(

			vec2(leftX, bottomY + height),
			vec2(leftX, bottomY),
			vec2(leftX + width, bottomY),
			vec2(leftX + width, bottomY + height)
		);


		GLsizei textureX = leftX * m_Width;
		GLsizei textureY = bottomY * m_Height;

		se_log("Submitting texture data to position " << leftX * m_Width << ", " << bottomY * m_Height << " with dimensions " << int_width << "x" << int_height)

		glActiveTexture(GL_TEXTURE0); //Set active texture to 0
		glBindTexture(GL_TEXTURE_2D, texture.getTextureID()); //Bind texture to 2D texture target

		m_textureCoords.emplace(textureID, coords);
		glTexSubImage2D(GL_TEXTURE_2D, 0, textureX, textureY, int_width, int_height, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
	}
	else {
		se_error("Non-uniform texture dimensions not yet supported");
	}

	texturesInAtlas++;
}

Shmingo::QuadTextureCoords Shmingo::TextureAtlas::getTextureCoords(size_t textureID){
	if (textureID < texturesInAtlas) {
		return m_textureCoords[textureID];
	}
	se_error("Texture ID not found in atlas");
	return Shmingo::QuadTextureCoords(vec2(0,0),vec2(0,0),vec2(0,0),vec2(0,0));
}
