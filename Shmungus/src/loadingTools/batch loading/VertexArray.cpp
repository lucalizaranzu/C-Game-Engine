#include <sepch.h>
#include "VertexArray.h"

#include "ModelTools.h"

VertexArray::VertexArray(){

	glGenVertexArrays(1, &vaoID); //Generate a VAO using the member variable vaoID

	glGenBuffers(1, &indexVboID); //Generates a Index buffer for VAO with member data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVboID); //Bind index buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, MAX_FLOATS * sizeof(int), nullptr, GL_DYNAMIC_DRAW); //Sets buffer data arguments for index buffer 
}

//Only needed for single VBO arrays
InterleavedVertexArray::InterleavedVertexArray() {

	glGenBuffers(1, &vertexVboID); //Generates a vertex buffer for VAO with member data
	glBindBuffer(GL_ARRAY_BUFFER, vertexVboID); //Bind vertex buffer
	glBufferData(GL_ARRAY_BUFFER, MAX_FLOATS * sizeof(float), nullptr, GL_DYNAMIC_DRAW); //Sets buffer data arguments for vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0); //Unbind vertex buffer
}


VertexArray::~VertexArray() {

}

void VertexArray::bind(){
	glBindVertexArray(vaoID);
}

void VertexArray::unbind() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}



void InterleavedVertexArray::bindVerticesVbo() {
	bind();
	glBindBuffer(GL_ARRAY_BUFFER, vertexVboID);
}

void VertexArray::bindIndicesVbo() {
	bind();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVboID);
}

//Texture binding functions


void VertexArray::declareTextureSlot(Texture2D texture, GLuint slot) {

	//Declares the size of the array so we don't have to always loop through 32 slots
	if ((slot + 1) > maxTextureIndex) {
		maxTextureIndex = slot + 1;
	}
	//Inserts texture ID into array with index being slot. can be overriden
	auto it = textureList.begin();
	textureList.emplace(std::next(it,slot),texture);
	texture.setVaoLocalSlot(slot);
}

void VertexArray::bindTextures() {

	//Loops through all the texture IDs and binds them to their slot according to their index in the array
	for (int i = 0; i < (int)maxTextureIndex; i++) {
		textureList.at(i).bind();
	}
}





//Subclass constructors

DefaultVertexArray::DefaultVertexArray(){

	attribAmount = 3;

	//Set attrib pointers unique to this type of vertex array
	bindVerticesVbo();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(DefaultVertex), (const void*)offsetof(DefaultVertex, positions));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(DefaultVertex), (const void*)offsetof(DefaultVertex, texCoords));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(DefaultVertex), (const void*)offsetof(DefaultVertex, normals));
	//unbind VAO
	unbind();
}

PositionsOnlyVertexArray::PositionsOnlyVertexArray() {

	attribAmount = 1;

	//Set attrib pointers unique to this type of vertex array
	bindVerticesVbo();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(PositionsOnlyVertex), (const void*)offsetof(PositionsOnlyVertex, positions));
	//unbind VAO
	unbind();

}

TexturedQuadVertexArray::TexturedQuadVertexArray() {

	attribAmount = 2;

	//Set attrib pointers unique to this type of vertex array
	bindVerticesVbo();
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedQuadVertex), (const void*)offsetof(TexturedQuadVertex, positions));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedQuadVertex), (const void*)offsetof(TexturedQuadVertex, texCoords));
	//unbind VAO
	unbind();

}

ColorQuadVertexArray::ColorQuadVertexArray() {

	attribAmount = 2;

	//Set attrib pointers unique to this type of vertex array
	bindVerticesVbo();
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(ColorQuadVertex), (const void*)offsetof(ColorQuadVertex, positions));
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(ColorQuadVertex), (const void*)offsetof(ColorQuadVertex, color));
	//unbind VAO
	unbind();

}


void TexturedQuadVertexArray::pushVertexData(TexturedQuadVertex* vertices, GLuint vertexAmt) {


	//Size of each vertex in bytes
	GLuint verticesByteSize = vertexAmt * sizeof(TexturedQuadVertex);
	int indexAmt = (int)vertexAmt * 3 / 2; //Convert vertex amount to index amount, for quads there will always be 3/2 as many indices as vertices
	GLuint indicesByteSize = indexAmt * sizeof(int);

	//Need to dynamically allocate to store custom size array, use unique ptr for safety
	std::unique_ptr<int[]> indexBuffer;
	auto indices = indexBuffer.get(); 
	//Generate indices for a quad following the pattern 0,1,3,3,1,2
	for (int i = 0; i < indexAmt; i += 6) {

		//Standard algorithm of 0,1,3,3,1,2 incremented
		int index1 = 0 + (int)vertexCount;
		int index2 = 1 + (int)vertexCount;
		int index3 = 2 + (int)vertexCount;
		int index4 = 3 + (int)vertexCount;

		//Set index buffer according to algorithm
		indices[i + 0] = index1;
		indices[i + 1] = index2;
		indices[i + 2] = index4;
		indices[i + 3] = index4;
		indices[i + 4] = index2;
		indices[i + 5] = index3;
	}

	//Bind the vertices vbo and push the vertex data
	bindVerticesVbo();
	glBufferSubData(GL_ARRAY_BUFFER, verticesEndIndex, verticesByteSize, vertices);
	//Bind the indices vbo and push the index data
	bindIndicesVbo();
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, indicesEndIndex, indicesByteSize, indices);

	//Increment the starting point of the vertex and index buffers
	verticesEndIndex += verticesByteSize; //Byte position of vertex end
	indicesEndIndex += indicesByteSize; //Byte position of index end

	//Increment the vertex and index counts
	vertexCount += vertexAmt;
	indexCount += indexAmt;

	unbind(); //Unbinds vao and vbos

}

void ColorQuadVertexArray::pushVertexData(ColorQuadVertex* vertices, GLuint vertexAmt) {

	//Size of vertex data in bytes
	GLuint verticesByteSize = vertexAmt * sizeof(ColorQuadVertex);
	int indexAmt = (int)vertexAmt * 3 / 2; //Convert vertex amount to index amount, for quads there will always be 3/2 as many indices as vertices
	GLuint indicesByteSize = indexAmt * sizeof(int);

	//Need to dynamically allocate to store custom size array, use unique ptr for safety
	std::unique_ptr<int[]> indexBuffer;
	auto indices = indexBuffer.get();
	//Generate indices for a quad following the pattern 0,1,3,3,1,2
	for (int i = 0; i < indexAmt; i += 6) {

		//Standard algorithm of 0,1,3,3,1,2 incremented
		int index1 = 0 + (int)vertexCount;
		int index2 = 1 + (int)vertexCount;
		int index3 = 2 + (int)vertexCount;
		int index4 = 3 + (int)vertexCount;

		//Set index buffer according to algorithm
		indices[i + 0] = index1;
		indices[i + 1] = index2;
		indices[i + 2] = index4;
		indices[i + 3] = index4;
		indices[i + 4] = index2;
		indices[i + 5] = index3;
	}

	//Bind the vertices vbo and push the vertex data
	bindVerticesVbo();
	glBufferSubData(GL_ARRAY_BUFFER, verticesEndIndex, verticesByteSize, vertices);
	//Bind the indices vbo and push the index data
	bindIndicesVbo();
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, indicesEndIndex, indicesByteSize, indices);

	//Increment the starting point of the vertex and index buffers
	verticesEndIndex += verticesByteSize; //Byte position of vertex end
	indicesEndIndex += indicesByteSize; //Byte position of index end

	//Increment the vertex and index counts
	vertexCount += vertexAmt;
	indexCount += indexAmt;

	unbind(); //Unbinds vao and vbos

	delete[] indices; //Delete index buffer after we're done with it
}









//Multi VBO array function
// s
EntityVertexArray::EntityVertexArray() {

	attribAmount = 3;

	glBindVertexArray(vaoID); //Bind VAO

	glGenBuffers(1, &positionsVboID); //Generates a vertex buffer for positions
	glGenBuffers(1, &texCoordsVboID); //Generates a vertex buffer for texCoords
	glGenBuffers(1, &textureIDVboID); //Generates a vertex buffer for textureIDs	

	glBindBuffer(GL_ARRAY_BUFFER, positionsVboID); //Bind vertex buffer
	glBufferData(GL_ARRAY_BUFFER, MAX_FLOATS * sizeof(float), nullptr, GL_DYNAMIC_DRAW); //Sets buffer data arguments for vertex buffer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr); //Sets vertex attribute pointer for positions

	glBindBuffer(GL_ARRAY_BUFFER, texCoordsVboID); //Bind vertex buffer
	glBufferData(GL_ARRAY_BUFFER, MAX_FLOATS * sizeof(float) * 2 / 3, nullptr, GL_DYNAMIC_DRAW); //Sets buffer data arguments for vertex buffer
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr); //Sets vertex attribute pointer for texCoords

	glBindBuffer(GL_ARRAY_BUFFER, textureIDVboID); //Bind vertex buffer
	glBufferData(GL_ARRAY_BUFFER, MAX_FLOATS * sizeof(float) / 3, nullptr, GL_DYNAMIC_DRAW); //Sets buffer data arguments for vertex buffer
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, nullptr); //Sets vertex attribute pointer for textureIDs

	glBindBuffer(GL_ARRAY_BUFFER, 0); //Unbind vertex buffer 
}


void EntityVertexArray::pushVertexData(std::shared_ptr<Entity> entity) {

	vertexOffsetMap.insert(std::make_pair(entity, verticesEndIndex)); //Is a normalized offset for ONE float, multiply to use for other VBOs
	indexOffsetMap.insert(std::make_pair(entity, indicesEndIndex)); //Sets the offset of the model data as the current vertices end index
	insertOrderVector.push_back(entity); //Pushes the entity into the insert order vector

	Model model = entity->getModel();

	GLuint vertexAmt = model.getVertexCount();
	GLuint indexAmt = model.getIndexCount();

	//Size of vertex data in bytes
	GLuint singleFloatByteSize = sizeof(float) * vertexAmt;
	GLuint indicesByteSize = indexAmt * sizeof(int);

	std::unique_ptr<int[]> newIndices = Shmingo::shiftIndices(model.getIndexData(), vertexCount, model.getIndexCount()); //Shifts the indices to match the new vertex offset

	//Bind the vertices vbo and push the vertex data
	glBindBuffer(GL_ARRAY_BUFFER, positionsVboID); //Bind vertex buffer
	glBufferSubData(GL_ARRAY_BUFFER, verticesEndIndex * 3, singleFloatByteSize * 3, model.getPositionData());

	glBindBuffer(GL_ARRAY_BUFFER, texCoordsVboID); //Bind vertex buffer
	glBufferSubData(GL_ARRAY_BUFFER, verticesEndIndex * 2, singleFloatByteSize * 2, model.getTextureCoords());

	glBindBuffer(GL_ARRAY_BUFFER, textureIDVboID); //Bind vertex buffer
	glBufferSubData(GL_ARRAY_BUFFER, verticesEndIndex, singleFloatByteSize, model.createTexIDArray().get());


	//Bind the indices vbo and push the index data
	bindIndicesVbo();
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, indicesEndIndex, indicesByteSize, newIndices.get());

	//Increment the starting point of the vertex and index buffers
	verticesEndIndex += singleFloatByteSize; //Byte position of vertex end
	indicesEndIndex += indicesByteSize; //Byte position of index end


	//Increment the vertex and index counts
	vertexCount += vertexAmt;
	indexCount += indexAmt;

	unbind(); //Unbinds vao and vbos
}

//Maybe put this on another thread? Or run some diagnostics with multiple entities comparing doing this with a compute shader
//vs doing this this way. If there is a inflection point, maybe change methods depending on size of entity buffer
//May need to change to a raw pointer in the future
void EntityVertexArray::removeVertexData(std::shared_ptr<Entity> entity){

	GLuint vertexRemovalIndex = vertexOffsetMap.at(entity);
	GLuint indexRemovalIndex = indexOffsetMap.at(entity);

	GLuint verticesRemovalSize = entity->getModel().getVerticesByteSize();
	GLuint indicesRemovalSize = entity->getModel().getIndicesByteSize();

	entityVec::iterator entityPos = std::find(insertOrderVector.begin(),
		insertOrderVector.end(), entity);
	 
	int vertexShiftByteSize = vertexOffsetMap[insertOrderVector.back()] - vertexRemovalIndex - verticesRemovalSize; //Gets the byte size of the data we are shifting
	int indexShiftByteSize = indexOffsetMap[insertOrderVector.back()] - indexRemovalIndex - indicesRemovalSize; //Gets the byte size of the data we are shifting

	GLuint vertexRemovalOffset = vertexOffsetMap[entity];
	GLuint indexRemovalOffset = indexOffsetMap[entity];

	//glBindBuffer(GL_COPY_READ_BUFFER, tempBuffer); //Bind temp buffer

	//Vertex buffer shift
	//glBindBuffer(GL_ARRAY_BUFFER, vertexVboID); //Bind vertex VBO
	glCopyBufferSubData(GL_ARRAY_BUFFER, GL_COPY_READ_BUFFER, vertexRemovalOffset + verticesRemovalSize, 0, vertexShiftByteSize); //Copies data after removal index to temp buffer
	glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_ARRAY_BUFFER, 0, vertexRemovalOffset, vertexShiftByteSize); //Copies data from tempBuffer back to vertex buffer at new offset

	//Index buffer shift
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexVboID); //Bind vertex VBO
	glCopyBufferSubData(GL_ELEMENT_ARRAY_BUFFER, GL_COPY_READ_BUFFER, indexRemovalOffset + indicesRemovalSize, 0, indexShiftByteSize); //Copies data after removal index to temp buffer
	glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_ELEMENT_ARRAY_BUFFER, 0, indexRemovalOffset, indexShiftByteSize); //Copies data from tempBuffer back to index buffer at new offset

	glBindBuffer(GL_COPY_READ_BUFFER, 0); //Unbind temp buffer

	//Shift map entries to represent new offset after shift
	for (auto it = entityPos; it != insertOrderVector.end() ; it++) {
		vertexOffsetMap[*it] -= verticesRemovalSize;
		indexOffsetMap[*it] -= indicesRemovalSize;
	}

	vertexCount -= entity->getModel().getVertexCount();
	indexCount -= entity->getModel().getIndexCount();

}