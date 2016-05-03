//Introductory matrix class

 #ifndef MYMATRIX__H 
 #define MYMATRIX__H 

#include <math.h>

#include "windows.h"

#include <gl/gl.h>            // standard OpenGL include
#include <gl/glu.h>           // OpenGL utilties
#include <glut.h>             // OpenGL utilties
#include <iostream>
#include "myFormulas.h"

class MyMatrix
{
	public:
		MyMatrix(void);
		~MyMatrix(void){;}

		void loadIdentity(void);

		void getGLModelviewMatrix(void);
		void getGLProjectionMatrix(void);

		void multiplyGLMatrix(void);
		void setGLMatrix(void);
		void MyMatrix::translateMatrix(float x, float y, float z);

		void MyMatrix::rotateZAxis(int degrees);

		void MyMatrix::printMatrix();

	private:

		void getGLMatrix(GLenum pname);

		GLfloat myMatrix[16];
};

#endif