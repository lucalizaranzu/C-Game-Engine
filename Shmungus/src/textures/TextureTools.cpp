#include <sepch.h>
#include <stb_image/stb_image.h>

#include "TextureTools.h"


Texture2D createTexture2D(std::string filePath){

	int width, height, channels;

	stbi_set_flip_vertically_on_load(1); //Flip texture to offset OpenGL flipping
	stbi_uc* textureData = stbi_load(filePath.c_str(), &width, &height, &channels, 0); //stbi_uc is a typdef of unsigned char*, can use to initialize texture

	Texture2D texture = Texture2D(textureData, width, height, channels);

	stbi_image_free(textureData); //Frees char pointer

	return texture;
}
