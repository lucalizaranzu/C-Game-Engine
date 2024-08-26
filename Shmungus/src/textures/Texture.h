#pragma once
	
#include <shmingoCore.h>

class Texture {

	friend class Model;

public:

	Texture();
	Texture(const Texture& other);

	~Texture();

	Texture& operator=(const Texture& other);

	GLuint getWidth() { return width; };
	GLuint getHeight() { return height; };
	GLuint& getTextureID() { return textureID; };

	GLuint getVaoLocalSlot() { return vaoLocalSlot; };
	void setVaoLocalSlot(GLuint id) { vaoLocalSlot = id; };

	void bind();

	void printTextureInfo() { se_log("Texture Address: " << &textureID); };

protected:

	inline GLuint getTextureIDValue() { return textureID; };

	GLuint textureID = 0;

	GLuint width = 0;
	GLuint height = 0;

	GLuint vaoLocalSlot = 0; //This is somewhat complicated, each VAO needs to have an ID to refer to this texture by

};

class Texture2D : public Texture {

public:

	Texture2D(unsigned char* textureData, GLuint width, GLuint height);

};
