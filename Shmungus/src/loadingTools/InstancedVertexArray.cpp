#include <sepch.h>
#include "InstancedVertexArray.h"
#include "MasterRenderer.h"


InstancedVertexArray::InstancedVertexArray(Shmingo::EntityType type, std::shared_ptr<Model> model) : instanceModel(model), entityType(type),
	perInstanceAttributeInfo(se_masterRenderer.getEntitySpecificInstanceAttributeInfoRef(type)){

	linkTexture(model->getTexture(), 0); //Links the texture to the VAO

	GLuint perInstanceAttributeAmount = se_masterRenderer.getEntitySpecificMajorInstanceAttribAmount(type); //Set total number of uniforms to be bound and unbound in the render method
	attribAmount = 2 + perInstanceAttributeAmount; //Set total number of attributes to be bound and unbound in the render method
	se_log("Instanced Attrib amount: " << perInstanceAttributeAmount);
	se_log("Total Attrib amount: " << attribAmount);
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

	//Set attributes for per instance data
	for (GLuint i = 0; i < perInstanceAttributeAmount; i++) {
		perInstanceVboIDs.emplace_back(0); //Adds 0 to vbo ids

		Shmingo::EntitySpecificInstanceDataInfo currentAttributeInfo = perInstanceAttributeInfo[i];

		GLuint currentAttribNumber = 2 + currentAttributeInfo.attributeNumber;

		GLuint adjustedAttribSize = currentAttributeInfo.size;
		if (currentAttributeInfo.size > 4) { //Done in case we have a size greater than 4, in which case we need to adjust the size to 4 to correctly set the stride
			adjustedAttribSize = 4;
		}

		perInstanceVboIDs[i] = 0;
		glGenBuffers(1, &perInstanceVboIDs[i]); //Generates a vertex buffer for instanced attributes
		glBindBuffer(GL_ARRAY_BUFFER, perInstanceVboIDs[i]); //Bind vertex buffer
		glBufferData(GL_ARRAY_BUFFER, 1000 * sizeof(float) * currentAttributeInfo.size, nullptr, GL_DYNAMIC_DRAW); //Sets buffer data arguments for vertex buffer, allocates space for 1000 entities, will have to change

		se_log("Setting pointer for real attribute " << currentAttribNumber << " with size " << adjustedAttribSize << ", stride " << currentAttributeInfo.size << ", and offset " << 0);

		glVertexAttribPointer(currentAttribNumber, adjustedAttribSize, GL_FLOAT, GL_FALSE, currentAttributeInfo.size * sizeof(float), (void*)0);
		glVertexAttribDivisor(currentAttribNumber, 1); //Sets vertex attribute divisor to once per instance

		if (currentAttributeInfo.size > 4) {

			GLuint fullSizeAttribs = currentAttributeInfo.size / 4;
			GLuint remainderAttribSize = currentAttributeInfo.size % 4;

			se_log("Setting pointer for real attribute " << currentAttribNumber << " with size " << currentAttributeInfo.size);

			for (GLuint j = 1; j < fullSizeAttribs; j++) {

				se_log("Setting pointer for real attribute " << currentAttribNumber + j << " with size 4" << adjustedAttribSize << ", stride " << currentAttributeInfo.size << ", and offset " << 0);


				glVertexAttribPointer(currentAttribNumber + j, 4, GL_FLOAT, GL_FALSE, currentAttributeInfo.size * sizeof(float), (void*)((sizeof(float) * (4 * j)))); //Set attrib pointer with correct offset and size 4
				glVertexAttribDivisor(currentAttribNumber + j, 1); //Sets vertex attribute divisor to once per vertex
			}

			if (remainderAttribSize != 0) {
				glVertexAttribPointer(currentAttribNumber + fullSizeAttribs, currentAttributeInfo.size * sizeof(float), GL_FLOAT, GL_FALSE,
					currentAttributeInfo.size * sizeof(float), (void*)(sizeof(float) * (4 * fullSizeAttribs)));
				glVertexAttribDivisor(currentAttribNumber + fullSizeAttribs, 1); //Sets vertex attribute divisor to once per vertex
			}
		}
		unbindBuffer();
	}

	glBindVertexArray(0); //Unbind vertex buffer 
}

//For shared pointer (unused)
void InstancedVertexArray::submitInstanceData(std::shared_ptr<InstancedEntity> instancedEntity) {

	const GLuint perInstanceInstanceAttribAmt = se_masterRenderer.getEntitySpecificMajorInstanceAttribAmount(entityType);

	instancedEntity->setOffsetInVao(instanceAmount);
	se_log("Offset in VAO: " << instancedEntity->getOffsetInVao());
	bindVao();

	for (GLuint i = 0; i < perInstanceInstanceAttribAmt; i++) {

		float* instancedData = instancedEntity->getInstanceData(i);

		glBindBuffer(GL_ARRAY_BUFFER, perInstanceVboIDs[i]); //Bind vertex buffer
		glBufferSubData(GL_ARRAY_BUFFER, instancedEntity->getOffsetInVao() * perInstanceAttributeInfo[i].size * sizeof(float),
			perInstanceAttributeInfo[i].size * sizeof(float), instancedData); //Sets buffer data arguments for vertex buffer	
	}

	instanceAmount += 1;
	indexCount += instanceModel->getIndexCount();
}


//For raw pointers
void InstancedVertexArray::submitInstanceData(InstancedEntity* instancedEntity) {

	const GLuint perInstanceInstanceAttribAmt = se_masterRenderer.getEntitySpecificMajorInstanceAttribAmount(entityType);

	instancedEntity->setOffsetInVao(instanceAmount);

	bindVao();


	for (GLuint i = 0; i < perInstanceInstanceAttribAmt; i++) {

		float* instancedData = instancedEntity->getInstanceData(i);
		GLuint size = perInstanceAttributeInfo[i].size;

		glBindBuffer(GL_ARRAY_BUFFER, perInstanceVboIDs[i]); //Bind vertex buffer
		glBufferSubData(GL_ARRAY_BUFFER, instancedEntity->getOffsetInVao() * perInstanceAttributeInfo[i].size * sizeof(float),
			perInstanceAttributeInfo[i].size * sizeof(float), instancedData); //Sets buffer data arguments for vertex buffer	
	}
	glBindBuffer(GL_ARRAY_BUFFER, perInstanceVboIDs[0]); //Unbind vertex buffer

	instanceAmount += 1;
	indexCount += instanceModel->getIndexCount();
}



void InstancedVertexArray::updateInstanceData(InstancedEntity* entity, GLuint attributePositionInArray){

	GLuint dataSize = perInstanceAttributeInfo[attributePositionInArray].size * sizeof(float);
	GLuint realAttributeNumber = perInstanceAttributeInfo[attributePositionInArray].attributeNumber;
	GLuint vaoOffset = entity->getOffsetInVao();

	glBindVertexArray(vaoID); //Bind VAO
	glBindBuffer(GL_ARRAY_BUFFER, perInstanceVboIDs[attributePositionInArray]); //Bind vertex buffer)
	glBufferSubData(GL_ARRAY_BUFFER, vaoOffset * dataSize, dataSize, entity->getInstanceData(realAttributeNumber)); //Sets buffer data arguments for vertex buffer)
	glBindBuffer(GL_ARRAY_BUFFER, 0); //Unbind vertex buffer)
	glBindVertexArray(0); //Unbind VAO
}

void InstancedVertexArray::removeInstancedData(GLuint offset){

	bindVao();

	const GLuint perInstanceInstanceAttribAmt = se_masterRenderer.getEntitySpecificMajorInstanceAttribAmount(entityType); //Get instanced attribute amount

	//Condition where there is only one entity in the vertex array
	if (instanceAmount == 1) {
		for (GLuint i = 0; i < perInstanceInstanceAttribAmt; i++) {

			glDeleteBuffers(1, &perInstanceVboIDs[i]); //Deletes vertex buffers
		}

		glDeleteVertexArrays(1, &vaoID); //Deletes VAO

		return; //Exits function, make sure to delete vertex array class after
	}

	//Condition where the entity being removed is the last entity in the vertex array
	if (offset == instanceAmount - 1) {

		//No need to do anything
	}

	else {
		//For instance data
		for (GLuint i = 0; i < perInstanceInstanceAttribAmt; i++) {

			glBindBuffer(GL_ARRAY_BUFFER, perInstanceVboIDs[i]); //Bind vertex buffer

			GLuint dataSize = perInstanceAttributeInfo[i].size * sizeof(float);

			copyBufferData((instanceAmount - 1) * dataSize,
				offset * dataSize, dataSize); //Copies data from last instance to the instance being removed
		}
	}
	instanceAmount -= 1; //Decrement instance amount
	indexCount -= instanceModel->getIndexCount(); //Decrement index count
	se_log("Instance amount: " << instanceAmount);
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

void InstancedVertexArray::copyBufferData(GLuint readOffset, GLuint writeOffset, GLuint size){

	glCopyBufferSubData(GL_ARRAY_BUFFER, GL_ARRAY_BUFFER, readOffset, writeOffset, size);

}