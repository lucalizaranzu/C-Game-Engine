#include <sepch.h>

#include "InstancedVertexArray.h"
#include "MasterRenderer.h"


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

TexturedQuadVertexArrayAtlas::TexturedQuadVertexArrayAtlas(Shmingo::TextureAtlas textureAtlas) : m_textureAtlas(textureAtlas){

	m_attribAmt = 2;
	m_indicesPerInstance = 6; //6 indices per quad

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
	glBufferData(GL_ARRAY_BUFFER,  sizeof(float) * 8, positionData, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0); // position

	glBindBuffer(GL_ARRAY_BUFFER, m_transformVboID);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(float) * m_maxVertexCount, nullptr, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0); // transform position
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(2 * sizeof(float))); // transform scale

	glVertexAttribDivisor(1, 1); //Set vertex attribute divisor to once per instance
	glVertexAttribDivisor(2, 1); //Set vertex attribute divisor to once per instance

	glBindBuffer(GL_ARRAY_BUFFER, m_texIDVboID);
	glBufferData(GL_ARRAY_BUFFER, m_maxVertexCount, nullptr, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 1, (void*)(0)); // texture ID
	glVertexAttribDivisor(3, 1); //Set vertex attribute divisor to once per instance

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	float indices[6] = { 0, 1, 3, 3, 1, 2 };

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(float), &indices, GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void TexturedQuadVertexArrayAtlas::init(){
	setTexCoordsUniforms(se_masterRenderer.getShader(se_MENU_SHADER));
}

void TexturedQuadVertexArrayAtlas::bindTextures(){
	glActiveTexture(GL_TEXTURE0);
	m_textureAtlas.bind();

	GLint textureUniformLocation = glGetUniformLocation(se_masterRenderer.getShader(se_MENU_SHADER)->getProgramID(), "textureAtlas");
	glUniform1i(textureUniformLocation, 0);
}

size_t TexturedQuadVertexArrayAtlas::submitQuad(Shmingo::Quad quad, uint8_t textureID){

	Shmingo::QuadTextureCoords texCoords = m_textureAtlas.getTextureCoords(textureID); // get texture coordinates for this ID


	float transformData[4] = {
		quad.position.x, quad.position.y,
		quad.size.x, quad.size.y
	};

	bindVao();

	glBindBuffer(GL_ARRAY_BUFFER, m_transformVboID);
	glBufferSubData(GL_ARRAY_BUFFER, m_instanceCount * sizeof(float), 4 * sizeof(float), transformData); //Put data in buffer

	glBindBuffer(GL_ARRAY_BUFFER, m_texIDVboID);
	glBufferSubData(GL_ARRAY_BUFFER, m_instanceCount, 0, &textureID); //Put data in buffer

	return m_instanceCount++; //Post increment quadCount and return the value before incrementing
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
	m_textureAtlas.addTexture(textureID, filePath);
}

void TexturedQuadVertexArrayAtlas::setTexCoordsUniforms(std::shared_ptr<ShaderProgram> shader){

	size_t textureAmt = m_textureAtlas.getTextureCount();

	vec4* textureCoords = new vec4[textureAmt];

	for (size_t i = 0; i < textureAmt; i++) {
		Shmingo::QuadTextureCoords coords = m_textureAtlas.getTextureCoords(i);
		textureCoords[i] = vec4(coords.bottomLeft, coords.topRight);
	}

	int uniformLocation = glGetUniformLocation(shader->getProgramID(), "atlasTexCoords");
}


