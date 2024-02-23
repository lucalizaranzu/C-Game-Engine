#pragma once


class Texture {

public:

	Texture();
	~Texture();

	GLuint getWidth() { return width; };
	GLuint getHeight() { return height; };

	void bind(GLuint slot);

protected:

	GLuint textureID = 0;


	GLuint width;
	GLuint height;

};

class Texture2D : public Texture {

public:

	Texture2D(unsigned char* textureData, GLuint width, GLuint height, GLuint channels);


};
