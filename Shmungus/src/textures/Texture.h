#pragma once
	
#include <shmingoCore.h>

class Texture {

public:

	Texture();
	~Texture();

	GLuint getWidth() { return width; };
	GLuint getHeight() { return height; };
	GLuint getTextureID() { return textureID; };

	GLuint* getTextureIDRef() { return &textureID; };

	void bind(int slot);

protected:

	GLuint textureID = 0;

	GLuint width = 0;
	GLuint height = 0;

};

class Texture2D : public Texture {

public:

	Texture2D(unsigned char* textureData, GLuint width, GLuint height, GLuint channels);

};
