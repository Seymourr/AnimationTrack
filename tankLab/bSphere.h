#include <math.h>

#include "windows.h"

#include <gl/gl.h>            // standard OpenGL include
#include <gl/glu.h>           // OpenGL utilties
#include <glut.h>             // OpenGL utilties
#include "objloader.h"


	class BoundingSphere
	{
	public:
		BoundingSphere(void);
		~BoundingSphere(void) { ; }

		void createBoundingSphere(ObjMesh *[], int);

		float radius = 0.0;
		float centerX = 0.0;
		float centerY = 0.0;
		float centerZ = 0.0;

	};
