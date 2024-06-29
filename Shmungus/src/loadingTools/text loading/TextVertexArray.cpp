#include <sepch.h>

#include "TextVertexArray.h"
#include "ShmingoApp.h"

//Vertex data for a quad
GLfloat positions[] = {
		0.0f,1.0f,
		0.0f,0.0f,
		1.0f,1.0f,
		1.0f,0.0f,
};

//Texture coordinates for a quad
GLfloat texCoords[] = {
		0.0f,0.0f,
		0.0f,1.0f,
		1.0f,0.0f,
		1.0f,1.0f,
};

float* textureTempBuffer = new float[500];
float* colorTempBuffer = new float[1500];
float* positionsTempBuffer = new float[1000];
float* scaleTempBuffer = new float[500];

TextVertexArray::TextVertexArray(){

	glGenVertexArrays(1, &vaoID); //Creates VAO using member variable

	glGenBuffers(1, &vertexPositionsVboID); //Generates a vertex buffer for positions
	glGenBuffers(1, &texCoordsVboID); //Generates a vertex buffer for texCoords
	glGenBuffers(1, &eboID); //Generates a vertex buffer for indices

	glBindVertexArray(vaoID); //Bind VAO

	//Set up vertex attributes

	//Static vertex attributes
	glBindBuffer(GL_ARRAY_BUFFER, vertexPositionsVboID);
	glBufferStorage(GL_ARRAY_BUFFER, 2 * 4 * sizeof(float), positions, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, texCoordsVboID);
	glBufferStorage(GL_ARRAY_BUFFER, 2 * 4 * sizeof(float), texCoords, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);


	//Per instance vertex attributes
	glBindBuffer(GL_ARRAY_BUFFER, charTextureVboID);
	glBufferData(GL_ARRAY_BUFFER, 1 * sizeof(float) * 2000, nullptr, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(float), nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, colorVboID);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(float) * 2000, nullptr, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, positionsVboID);
	glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(float) * 2000, nullptr, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, scaleVboID);
	glBufferData(GL_ARRAY_BUFFER, 1 * sizeof(float) * 2000, nullptr, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(5, 2, GL_FLOAT, GL_FALSE, 1 * sizeof(float), nullptr);

}

TextVertexArray::~TextVertexArray() {
	delete textureTempBuffer;
	delete colorTempBuffer;
	delete positionsTempBuffer;
	delete scaleTempBuffer;
}


void TextVertexArray::submitStaticText(TextBox textBox){


}

void submitTextLine(std::string text, int x, int y, float scale) {

	std::string::const_iterator c;
	int currentIndex = 0;

	int copyX = x;
	for (c = text.begin(); c != text.end(); c++){

		Shmingo::Character ch = se_application.getCharacterFontInfo(*c);

		if (*c == '\n') {
			y -= ((ch.Size.y)) * 1.3 * scale;
			x = copyX;
		}
		else if (*c == ' ') {
			x += (ch.Advance >> 6) * scale;
		}
		else {

			//Insert color logic here (&ffd or whatever)

			float xpos = x + ch.Bearing.x * scale;
			float ypos = y - (256 - ch.Bearing.y) * scale;

			positionsTempBuffer[2 * currentIndex] = x;
			positionsTempBuffer[2 * currentIndex + 1] = y;
			scaleTempBuffer[currentIndex] = scale; //TODO change attrib divisor and lock scale to per line
			textureTempBuffer[currentIndex] = ch.TextureID;

			x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
			currentIndex++;
		}
	}
}