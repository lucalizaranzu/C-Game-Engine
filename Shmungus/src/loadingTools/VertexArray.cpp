#include <sepch.h>
#include "VertexArray.h"

VertexArray::VertexArray(){

	glGenVertexArrays(1, &vaoID); //Generate a VAO using the member variable vaoID

	glBindVertexArray(vaoID); //Binds VAO
	glGenBuffers(1, &vertexVboID); //Generates a VBO for VAO with member data
	glBindBuffer(GL_ARRAY_BUFFER, vertexVboID); //Bind Vertex buffer
	glBufferData(GL_ARRAY_BUFFER, MAX_FLOATS * sizeof(float), nullptr, GL_DYNAMIC_DRAW); //Sets buffer data arguments for vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind vertex vbo

	glGenBuffers(1, &indexVboID); //Generates a Index buffer for VAO with member data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVboID); //Bind index buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, MAX_FLOATS * sizeof(int), nullptr, GL_DYNAMIC_DRAW); //Sets buffer data arguments for index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //Unbind index buffer
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

void VertexArray::bindVerticesVbo() {
	bind();
	glBindBuffer(GL_ARRAY_BUFFER, vertexVboID);
}

void VertexArray::bindIndicesVbo() {
	bind();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVboID);
}

//Renders a square, known to work
void VertexArray::pushTestVertices() {

	GLfloat testVertices[] = { //Cube vertices
			-0.5f, 0.5f, 0.0f,//v0
			-0.5f, -0.5f, 0.0f,//v1
			0.5f, -0.5f, 0.0f,//v2
			0.5f, 0.5f, 0.0f,//v3
	};

	GLuint testIndices[] = { //Cube indices
		0,1,3,//top left triangle (v0, v1, v3)
		3,1,2//bottom right triangle (v3, v1, v2)
	};

	se_log("test indices size: " << sizeof(testIndices));

	bindVerticesVbo();

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); //This will be changed in other classes, changing how data is stored
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(testVertices), testVertices);

	bindIndicesVbo();
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(testIndices), testIndices);

	indexCount += 6;
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

EntityVertexArray::EntityVertexArray() {

	attribAmount = 3;

	//Set attrib pointers unique to this type of vertex array
	bindVerticesVbo();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(EntityVertexArray), (const void*)offsetof(EntityVertex, positions));
	glVertexAttribPointer(1, 2, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(EntityVertexArray), (const void*)offsetof(EntityVertex, texCoords));
	glVertexAttribPointer(2, 1, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(EntityVertexArray), (const void*)offsetof(EntityVertex, textureID));
	//unbind VAO
	unbind();

}

void TexturedQuadVertexArray::pushVertexData(TexturedQuadVertex* vertices, GLuint vertexAmt) {


	//Size of each vertex in bytes
	GLuint verticesByteSize = vertexAmt * sizeof(TexturedQuadVertex);
	int indexAmt = (int)vertexAmt * 3 / 2; //Convert vertex amount to index amount, for quads there will always be 3/2 as many indices as vertices
	GLuint indicesByteSize = indexAmt * sizeof(int);

	int* indices = new int[indexAmt]; //Need to dynamically allocate to store custom size array
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

void ColorQuadVertexArray::pushVertexData(ColorQuadVertex* vertices, GLuint vertexAmt) {

	//Size of vertex data in bytes
	GLuint verticesByteSize = vertexAmt * sizeof(ColorQuadVertex);
	int indexAmt = (int)vertexAmt * 3 / 2; //Convert vertex amount to index amount, for quads there will always be 3/2 as many indices as vertices
	GLuint indicesByteSize = indexAmt * sizeof(int);

	int* indices = new int[indexAmt]; //Need to dynamically allocate to store custom size array
	//Generate indices for a quad following the pattern 0,1,3,3,1,2
	for (int i = 0; i < indexAmt; i+=6) {

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


void EntityVertexArray::pushVertexData(std::shared_ptr<Entity> entity) {

	vertexOffsetMap.insert(std::make_pair(entity, verticesEndIndex)); //Sets the offset of the model data as the current vertices end index
	indexOffsetMap.insert(std::make_pair(entity, indicesEndIndex)); //Sets the offset of the model data as the current vertices end index

	Model model = entity->getModel();

	GLuint vertexAmt = model.getVertexCount();
	GLuint indexAmt = model.getIndexCount();

	//Size of vertex data in bytes
	GLuint verticesByteSize = vertexAmt * sizeof(EntityVertex);
	GLuint indicesByteSize = indexAmt * sizeof(int);

	//Bind the vertices vbo and push the vertex data
	bindVerticesVbo();
	glBufferSubData(GL_ARRAY_BUFFER, verticesEndIndex, verticesByteSize, model.getVertexData());
	//Bind the indices vbo and push the index data
	bindIndicesVbo();
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, indicesEndIndex, indicesByteSize, model.getIndexData());

	//Increment the starting point of the vertex and index buffers
	verticesEndIndex += verticesByteSize; //Byte position of vertex end
	indicesEndIndex += indicesByteSize; //Byte position of index end

	//Increment the vertex and index counts
	vertexCount += vertexAmt;
	indexCount += indexAmt;

	unbind(); //Unbinds vao and vbos
}

//Maybe put this on another thread?
void EntityVertexArray::removeVertexData(std::shared_ptr<Entity> entity){

	GLuint vertexRemovalIndex = vertexOffsetMap.at(entity);
	GLuint indexRemovalIndex = indexOffsetMap.at(entity);

	GLuint verticesRemovalSize = entity->getModel().getVerticesByteSize();
	GLuint indicesRemovalSize = entity->getModel().getIndicesByteSize();

	entityVec::iterator entityPos = std::find(insertOrderVector.begin(),
		insertOrderVector.end(), entity);

	//TODO: actually shift data




	//Im looking to see if I can run something like this in a compute shader since this is a long for loop
	for (auto it = entityPos; it != insertOrderVector.end() ; it++) {
		vertexOffsetMap[*it] -= verticesRemovalSize;
		indexOffsetMap[*it] -= indicesRemovalSize;
	}

	vertexCount -= entity->getModel().getVertexCount();
	indexCount - +entity->getModel().getIndexCount();
}