//Introductory matrix class

#include <math.h>

#include "windows.h"

#include <gl/gl.h>            // standard OpenGL include
#include <gl/glu.h>           // OpenGL utilties
#include <glut.h>             // OpenGL utilties

const double M_PI = 3.14159265358979323846;
#define DEG2RAD(x) ((x)*M_PI/180.0)
#define RAD2DEG(x) ((x)*180.0/M_PI)

namespace MyMathLab
{

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
		
		void translateMatrix(float, float, float);
		void rotateZAxis(int);
	
		void MyMatrix::printMatrix();
	private:

		void getGLMatrix(GLenum pname);

		GLfloat myMatrix[16];
};


}