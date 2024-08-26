#pragma once

#include <ShmingoCore.h>

#include "TextureAtlas.h"
#include "ShaderProgram.h"

class InstancedVertexArray {

public:

	void cleanUp();

	size_t getInstanceCount(){ return m_instanceCount; }
	GLsizei getAttribAmt(){ return m_attribAmt; }
	size_t getIndexCount(){ return m_instanceCount; }

	virtual void bindTextures() = 0;

	void bindVao();

	GLuint& getVaoID(){ return m_vaoID; }


protected:

	GLsizei m_attribAmt = 0; //Will depend based on child class

	GLuint m_vaoID = 0; //Vertex Array Object ID
	GLuint m_eboID = 0; //Element Buffer Object ID

	size_t m_indicesPerInstance = 0; //Amount of indices per instance

	size_t m_instanceCount = 0;
	size_t m_maxVertexCount = 200; //Maximum vertex count

};


class TexturedQuadVertexArray : public InstancedVertexArray {

public:

	TexturedQuadVertexArray();

	void bindTextures() override {};

	//New system of getting index in the vertex array, it will now be returned instead of being set in the function because the quad will not be storing an index
	size_t submitQuad(Shmingo::Quad quad, GLuint textureID);
	void removeQuad(size_t index);

	void declareTexture(Texture2D texture, GLuint textureID); //Declare a texture and its corresponding ID


private:

	std::vector<Texture2D> m_textures; //Contains texture information for the vertex array, when using an atlas this should only contain one texture

};

class TexturedQuadVertexArrayAtlas : public InstancedVertexArray {

public:

	TexturedQuadVertexArrayAtlas(Shmingo::TextureAtlas textureAtlas);

	void init();
	void bindTextures() override;

	size_t submitQuad(Shmingo::Quad quad, uint8_t textureID);

	void changeQuadTexture(size_t index, uint8_t textureID);
	void removeQuad(size_t index);

	//Matches the textureID to the texture coordinates in the texture atlas
	void declareTexture(size_t textureID, std::string filePath);

	GLuint& getTextureID(){ return m_textureAtlas.getTextureID(); }

private:

	void setTexCoordsUniforms(std::shared_ptr<ShaderProgram> shader);

	GLuint m_positionsVboID = 0;
	GLuint m_transformVboID = 0;
	GLuint m_texIDVboID = 0;

	Shmingo::TextureAtlas m_textureAtlas;

};