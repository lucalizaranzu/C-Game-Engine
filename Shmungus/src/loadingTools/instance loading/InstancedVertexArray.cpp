#include <sepch.h>

#include "InstancedVertexArray.h"
#include "MasterRenderer.h"
#include "TextureTools.h"


void InstancedVertexArray::cleanUp(){
	glDeleteVertexArrays(1, &m_vaoID);
}

void InstancedVertexArray::bindVao(){
	glBindVertexArray(m_vaoID);
}

TexturedQuadVertexArray::TexturedQuadVertexArray(){

	glGenBuffers(1, &m_vaoID);
	glBindVertexArray(m_vaoID);
	

	glBindVertexArray(0);
}

TexturedQuadVertexArrayAtlas::TexturedQuadVertexArrayAtlas(std::shared_ptr<Shmingo::TextureAtlas> textureAtlas) : m_textureAtlas(textureAtlas){

	m_attribAmt = 2;

	glGenVertexArrays(1, &m_vaoID);

	glGenBuffers(1, &m_transformVboID);
	glGenBuffers(1, &m_positionsVboID);
	glGenBuffers(1, &m_texIDVboID);
	glGenBuffers(1, &m_eboID);

	glBindVertexArray(m_vaoID);

	float positionData[8] = {
		0,1,
		0,0,
		1,0,
		1,1
	};

	glBindBuffer(GL_ARRAY_BUFFER, m_positionsVboID);
	glBufferData(GL_ARRAY_BUFFER,  sizeof(float) * 8, &positionData, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0); // position

	glBindBuffer(GL_ARRAY_BUFFER, m_transformVboID);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(float) * m_maxVertexCount, nullptr, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0); // transform position
	glVertexAttribDivisor(1, 1); //Set vertex attribute divisor to once per instance
	
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float))); // transform scale
	glVertexAttribDivisor(2, 1); //Set vertex attribute divisor to once per instance

	glBindBuffer(GL_ARRAY_BUFFER, m_texIDVboID);
	glBufferData(GL_ARRAY_BUFFER, m_maxVertexCount * sizeof(uint8_t), nullptr, GL_DYNAMIC_DRAW);

	glVertexAttribIPointer(3, 1, GL_UNSIGNED_BYTE, 0, (void*)(0)); // texture ID
	glVertexAttribDivisor(3, 1); //Set vertex attribute divisor to once per instance

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	int indices[6] = { 0, 1, 3, 3, 1, 2 };

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(int), &indices, GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void TexturedQuadVertexArrayAtlas::init(){
	se_masterRenderer.getShader(se_MENU_SHADER)->start();
	setTexCoordsUniforms(se_masterRenderer.getShader(se_MENU_SHADER));
	se_masterRenderer.getShader(se_MENU_SHADER)->stop();
}

void TexturedQuadVertexArrayAtlas::bindTextures(){
	glActiveTexture(GL_TEXTURE0);

	GLint textureUniformLocation = glGetUniformLocation(se_masterRenderer.getShader(se_MENU_SHADER)->getProgramID(), "textureAtlas");

	//Texture2D funnyTexture = Shmingo::createTexture2D("funnyimage.png");
	m_textureAtlas->bind();

	glUniform1i(textureUniformLocation, 0);
	//setTexCoordsUniforms(se_masterRenderer.getShader(se_MENU_SHADER));


}

size_t TexturedQuadVertexArrayAtlas::submitQuad(Shmingo::Quad quad, uint8_t textureID){

	Shmingo::QuadTextureCoords texCoords = m_textureAtlas->getTextureCoords(textureID); // get texture coordinates for this ID


	float transformData[4] = {
		quad.position.x, quad.position.y,
		quad.size.x, quad.size.y
	};

	bindVao();

	glBindBuffer(GL_ARRAY_BUFFER, m_transformVboID);
	glBufferSubData(GL_ARRAY_BUFFER, m_instanceCount * 4 * sizeof(float), 4 * sizeof(float), &transformData); //Put data in buffer

	glBindBuffer(GL_ARRAY_BUFFER, m_texIDVboID);
	glBufferSubData(GL_ARRAY_BUFFER, m_instanceCount * sizeof(uint8_t), sizeof(uint8_t), &textureID); // Update with correct offset and size

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	return m_instanceCount++; //Post increment quadCount and return the value before incrementing
}

void TexturedQuadVertexArrayAtlas::resubmitQuad(size_t index, Shmingo::Quad quad, uint8_t textureID){
	Shmingo::QuadTextureCoords texCoords = m_textureAtlas->getTextureCoords(textureID); // get texture coordinates for this ID


	float transformData[4] = {
		quad.position.x, quad.position.y,
		quad.size.x, quad.size.y
	};

	bindVao();

	glBindBuffer(GL_ARRAY_BUFFER, m_transformVboID);
	glBufferSubData(GL_ARRAY_BUFFER, index * 4 * sizeof(float), 4 * sizeof(float), &transformData); //Put data in buffer

	glBindBuffer(GL_ARRAY_BUFFER, m_texIDVboID);
	glBufferSubData(GL_ARRAY_BUFFER, index * sizeof(uint8_t), sizeof(uint8_t), &textureID); // Update with correct offset and size

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void TexturedQuadVertexArrayAtlas::changeQuadTexture(size_t index, uint8_t textureID){

	bindVao();
	glBindBuffer(GL_ARRAY_BUFFER, m_texIDVboID);
	glBufferSubData(GL_ARRAY_BUFFER, index, 1, &textureID); //Put data in buffer

}

void TexturedQuadVertexArrayAtlas::removeQuad(size_t index){

	if (m_instanceCount == 0) {
		se_error("No quads to remove");
		return;
	}
	else if (m_instanceCount == 1 || index == m_instanceCount - 1) {
		m_instanceCount--; //Reset quad count
		return;
	}
	else {

		glBindBuffer(GL_ARRAY_BUFFER, m_transformVboID); //Copy last quad to the position of quad being removed
		glCopyBufferSubData(GL_ARRAY_BUFFER, GL_ARRAY_BUFFER, (m_instanceCount - 1) * 4 * sizeof(float), index * 4 * sizeof(float), 4 * sizeof(float));

		glBindBuffer(GL_ARRAY_BUFFER, m_texIDVboID); //Copy last quad to the position of quad being removed
		glCopyBufferSubData(GL_ARRAY_BUFFER, GL_ARRAY_BUFFER, (m_instanceCount - 1), index, 1);

		m_instanceCount--;
		m_instanceCount -= 6;

		return;
		
	}
}

void TexturedQuadVertexArrayAtlas::declareTexture(size_t textureID, std::string filePath){
	m_textureAtlas->addTexture(textureID, filePath);
}

void TexturedQuadVertexArrayAtlas::setTexCoordsUniforms(std::shared_ptr<ShaderProgram> shader){

	size_t textureAmt = m_textureAtlas->getTextureCount();

	float* textureCoords = new float[16 * 4];
	for (size_t i = 0; i < textureAmt; i++) {
		Shmingo::QuadTextureCoords coords = m_textureAtlas->getTextureCoords(i);
		textureCoords[4 * i + 0] = coords.bottomRight.x - coords.bottomLeft.x;
		textureCoords[4 * i + 1] = coords.topRight.y - coords.bottomRight.y;
		textureCoords[4 * i + 2] = coords.bottomLeft.x;
		textureCoords[4 * i + 3] = coords.bottomLeft.y;

	}

	int uniformLocation = glGetUniformLocation(shader->getProgramID(), "atlasTexCoords");
	glUniform4fv(uniformLocation, 16, textureCoords);

	delete[] textureCoords;

}


