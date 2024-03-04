#include <sepch.h>

#include "TextureTools.h"
#define STB_IMAGE_IMPLEMENTATION

std::shared_ptr<Texture2D> createTexture2D(std::string filePath){

	std::string realFilePath = "assets/textures/" + filePath;

	int width, height, channels;

	stbi_set_flip_vertically_on_load(1); //Flip texture to offset OpenGL flipping
	stbi_uc* textureData = stbi_load(realFilePath.c_str(), &width, &height, &channels, 0); //stbi_uc is a typdef of unsigned char*, can use to initialize texture

	if (!textureData) {
		std::string errorMsg = "Failed to load texture: ";
		errorMsg += filePath;
		se_log(errorMsg);
		throw std::runtime_error(errorMsg);
	}

	width = (GLuint)width;
	height = (GLuint)height;
	channels = (GLuint)channels;

	GLuint uWidth = width;
	GLuint uHeight = height;

	std::shared_ptr<Texture2D> texture = std::make_shared<Texture2D>(textureData, uWidth, uHeight, channels); //Creates texture object

	stbi_image_free(textureData); //Frees char 
	return texture;
}
