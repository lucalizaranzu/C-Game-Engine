#include <sepch.h>
#include "Texture.h"

#include <stb_image/stb_image.h>

Texture::Texture() {
	glCreateTextures(GL_TEXTURE_2D, 1, &textureID);
}

Texture::~Texture() {

	glDeleteTextures(1, &textureID);
}

Texture2D::Texture2D(unsigned char* textureData, GLuint width, GLuint height, GLuint channels) : Texture() {

	glTextureStorage2D(textureID, 1, GL_RGB8, width, height);

	//Texture parameters
	glTexParameteri(textureID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(textureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTextureSubImage2D(textureID, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, textureData);
	glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::bind(GLuint slot){
	//May need to change slot and add as parameter eventually
	glBindTextureUnit(slot, textureID);
}
