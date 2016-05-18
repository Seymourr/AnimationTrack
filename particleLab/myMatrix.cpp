#include "myMatrix.h"


//Default constructor
MyMatrix::MyMatrix(void)
{
	this->loadIdentity();
}

//Load identity matrix into this object
void MyMatrix::loadIdentity(void)
{
	static GLfloat identityMatrix[16] =
	{
		1.0,0.0,0.0,0.0,
		0.0,1.0,0.0,0.0,
		0.0,0.0,1.0,0.0,
		0.0,0.0,0.0,1.0
	};
	memcpy(this->myMatrix, identityMatrix, sizeof(identityMatrix));
}

// Returns any matrix (used by other functions: do use directly)
void MyMatrix::getGLMatrix(GLenum pname) 
{
	//remember, you should have a GLfloat myMatrix[16]; defined
	//as a member of your class (see above)
	glGetFloatv(pname,this->myMatrix);
}

// Returns the current modelview matrix
void MyMatrix::getGLModelviewMatrix(void) 
{
	getGLMatrix(GL_MODELVIEW_MATRIX);
}

// Returns the current projection matrix
void MyMatrix::getGLProjectionMatrix(void) 
{
	getGLMatrix(GL_PROJECTION_MATRIX);
}

// Multiply our matrix by the current OpenGL one
void MyMatrix::multiplyGLMatrix(void) 
//Remember to first set the current GL matrix by using the command:
// glMatrixMode()!!!
{
	glMultMatrixf(this->myMatrix);
}

// Set the OpenGL matrix to that currently specified in myMatrix
void MyMatrix::setGLMatrix(void)
//Remember to first set the current GL matrix by using the command:
// glMatrixMode()!!!
{
	
	glLoadMatrixf(this->myMatrix);
}

//Translate the matrix currently loaded in opengl with the translation matrix created by xyz
void MyMatrix::translateMatrix(float x, float y, float z) {
	//assuming matrix is 4x4..
	this->loadIdentity();
	this->myMatrix[12] += x;
	this->myMatrix[13] += y;
	this->myMatrix[14] += z;
	
	this->multiplyGLMatrix();
}

//Rotate the matrix currently loaded in opengl with the degree given by the parameter
void MyMatrix::rotateZAxis(int degrees) {
	float rad = DEG2RAD(degrees);


	float c = cos(rad);
	float s = sin(rad);

	this->loadIdentity();

	this->myMatrix[0] = c;
	this->myMatrix[1] = s;
	this->myMatrix[4] = -s;
	this->myMatrix[5] = c;

	this->multiplyGLMatrix();
}

//Print out this matrix in a nice, readable way
void MyMatrix::printMatrix() {
	std::cout << std::endl;
	int cnt = 0;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			std::cout << this->myMatrix[cnt] << " ";
			cnt += 1;
		}
		std::cout << std::endl;
	}
}
