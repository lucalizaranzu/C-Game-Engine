#include <sepch.h>
#include "InstancedVertexArray.h"
#include "MasterRenderer.h"


InstancedVertexArray::InstancedVertexArray(Shmingo::EntityType type, std::shared_ptr<Model> model) : instanceModel(model), entityType(type) {

	linkTexture(model->getTexture(), 0); //Links the texture to the VAO

	GLuint instancedAttribAmount = se_masterRenderer.getEntitySpecificAttribAmount(type); //Set total number of attributes to be bound and unbound in the render method
	attribAmount = 3 + instancedAttribAmount; //Set total number of attributes to be bound and unbound in the render method

	GLuint modelVertexCount = model->getVertexCount();

	glGenVertexArrays(1, &vaoID); //Creates VAO using member variable

	glGenBuffers(1, &vertexPositionsVboID); //Generates a vertex buffer for positions
	glGenBuffers(1, &texCoordsVboID); //Generates a vertex buffer for texCoords
	glGenBuffers(1, &eboID); //Generates a vertex buffer for indices

	glBindVertexArray(vaoID); //Bind VAO

	glBindBuffer(GL_ARRAY_BUFFER, vertexPositionsVboID);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(float) * modelVertexCount, model->getPositionData(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, texCoordsVboID);
	glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(float) * modelVertexCount, model->getTextureCoords(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, model->getIndexCount() * sizeof(int), model->getIndexData(), GL_STATIC_DRAW);

	auto instancedAttributeInfo = se_masterRenderer.getEntityVertexAttributeInfo(type);

	for (int i = 0; i < 15; i++) {
		se_log(model->getPositionData()[i] << " ");
	}


	for (GLuint i = 0; i < instancedAttribAmount; i++) {

		instancedVboIDs.emplace_back(0); //Adds 0 to vbo ids

		Shmingo::EntitySpecificVertexDataInfo currentAttributeInfo = instancedAttributeInfo[i];
		instancedVboIDs[i] = 0;
		glGenBuffers(1, &instancedVboIDs[i]); //Generates a vertex buffer for instanced attributes
		glBindBuffer(GL_ARRAY_BUFFER, instancedVboIDs[i]); //Bind vertex buffer

		glBufferData(GL_ARRAY_BUFFER, MAX_FLOATS * sizeof(float) * currentAttributeInfo.size, nullptr, GL_DYNAMIC_DRAW); //Sets buffer data arguments for vertex buffer
		glVertexAttribPointer(2 + currentAttributeInfo.vaoIndex, currentAttributeInfo.size, GL_FLOAT, GL_FALSE, currentAttributeInfo.size * sizeof(float), nullptr);
		glVertexAttribDivisor(2 + currentAttributeInfo.vaoIndex, 1); //Sets vertex attribute divisor to once per instance
	}

	glBindVertexArray(0); //Unbind vertex buffer 
}

void InstancedVertexArray::submitInstanceData(std::shared_ptr<InstancedEntity> instancedEntity) {

	glBindVertexArray(vaoID); //Bind VAO

	const GLuint instancedAttribAmt = se_masterRenderer.getEntitySpecificAttribAmount(entityType);

	instancedEntity->setOffsetInVao(instanceAmount);

	bindVao();

	std::vector<Shmingo::EntitySpecificVertexDataInfo> instancedAttributeInfo = se_masterRenderer.getEntityVertexAttributeInfo(entityType);
	for (int i = 0; i < instancedAttributeInfo.size(); i++) {
		se_log("Instanced attribute info #" << i << ": " << instancedAttributeInfo[i].name << " - " << instancedAttributeInfo[i].size << " - " << instancedAttributeInfo[i].vaoIndex << " - " << instancedAttributeInfo[i].localFloatOffset << std::endl);
	}

	for (GLuint i = 0; i < instancedAttribAmt; i++) {

		float* instancedData = instancedEntity->getVertexData(i, instancedAttributeInfo);

		glBindBuffer(GL_ARRAY_BUFFER, instancedVboIDs[i]); //Bind vertex buffer
		glBufferSubData(GL_ARRAY_BUFFER, instancedEntity->getOffsetInVao() * instancedAttributeInfo[i].size * sizeof(float),
			instancedAttributeInfo[i].size * sizeof(float), instancedData); //Sets buffer data arguments for vertex buffer	
	}

	instanceAmount += 1;
	indexCount += instanceModel->getIndexCount();
}

void InstancedVertexArray::submitInstanceData(InstancedEntity* instancedEntity) {

	glBindVertexArray(vaoID); //Bind VAO

	const GLuint instancedAttribAmt = se_masterRenderer.getEntitySpecificAttribAmount(entityType);

	instancedEntity->setOffsetInVao(instanceAmount);

	bindVao();

	std::vector<Shmingo::EntitySpecificVertexDataInfo> instancedAttributeInfo = se_masterRenderer.getEntityVertexAttributeInfo(entityType);

	for (GLuint i = 0; i < instancedAttribAmt; i++) {

		float* instancedData = instancedEntity->getVertexData(i, instancedAttributeInfo);

		glBindBuffer(GL_ARRAY_BUFFER, instancedVboIDs[i]); //Bind vertex buffer
		glBufferSubData(GL_ARRAY_BUFFER, instancedEntity->getOffsetInVao() * instancedAttributeInfo[i].size * sizeof(float),
			instancedAttributeInfo[i].size * sizeof(float), instancedData); //Sets buffer data arguments for vertex buffer	
	}

	instanceAmount += 1;
	indexCount += instanceModel->getIndexCount();
}



void InstancedVertexArray::linkTexture(Texture2D texture, GLuint slot) {

	//Declares the size of the array so we don't have to always loop through 32 slots
	if ((slot + 1) > maxTextureIndex) {
		maxTextureIndex = slot + 1;
	}

	//Inserts texture ID into array with index being slot. can be overriden
	auto it = textureList.begin();
	textureList.emplace(std::next(it, slot), texture);
	texture.setVaoLocalSlot(slot);
	//std::cout << "ID in link: ";
}

void InstancedVertexArray::bindTextures() {
	bindVao();
	//Loops through all the texture IDs and binds them to their slot according to their index in the array
	for (int i = 0; i < (int)maxTextureIndex; i++) {
		textureList.at(i).bind();
	}
}