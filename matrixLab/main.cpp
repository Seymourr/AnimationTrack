#define WIN32_LEAN_AND_MEAN

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(linker, "/subsystem:console")

#include "windows.h"
#include <stdlib.h>

#include <gl/gl.h>            // standard OpenGL include
#include <gl/glu.h>           // OpenGL utilties
#include "glut.h"             // OpenGL utilties


#include <iostream>
#include "myQuat.h" //Perhaps nicer with guards/include once for matrix & vec

using namespace MyMathLab;

int rotateAmount = 0;
int rotateIncrement = 5;

//prototypes for our callback functions
void draw(void);    //our drawing routine
void idle(void);    //what to do when nothing is happening
void key(unsigned char k, int x, int y);  //handle key presses
void reshape(int width, int height);      //when the window is resized
void init_drawing(void);                  //drawing intialisation

void DrawVector(MyPosition&, MyVector&);
void DrawLine(MyPosition&, MyPosition&);
void drawBasis(void);
void drawOrig(void);

/**
*	Draw a square 2 units long/high at the current translated coordinates. 
*/
void draw_square(void) {
	glBegin(GL_POLYGON);
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(-1.0, 1.0, 0.0);
		glColor3f(0.0, 1.0, 0.0);
		glVertex3f(-1.0, -1.0, 0.0);
		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(1.0, -1.0, 0.0);
		glColor3f(1.0, 1.0, 1.0);
		glVertex3f(1.0, 1.0, 0.0);
	glEnd();
}

/**
*	Draw a line between the specified positions, and a big dot at the end
*/
void DrawLine(MyPosition& startPos, MyPosition& endPos)
{
	glPushMatrix();
		float space = 0.25;
		glBegin(GL_LINES);
			glVertex3f(startPos.x, startPos.y, startPos.z);
			glVertex3f(endPos.x, endPos.y, endPos.z);

		glEnd();

		glTranslatef(endPos.x, endPos.y, endPos.z);
		glPointSize(8.0);
		

		glBegin(GL_POINTS);
			glVertex3f(0.0, 0.0, 0.0);
		glEnd();

	glPopMatrix();
}

/**
*	Draw the vector v1 starting from position startPos
*	This function will only work as long as the z coordinate for both positions is zero
*/
void DrawVector(MyPosition& startPos, MyVector& v1)
{
	float length = sqrt((v1.x * v1.x) + (v1.y * v1.y) + (v1.z * v1.z));
	MyVector v;
	if (length > 0.0) { v.x = v1.x / length; v.y = v1.y / length; v.z = v1.z / length; }
	else return;
	float d = (v.x * 0.0) + (v.y * 1.0) + (v.z * 0.0);
	float a = RAD2DEG(acos(d));
	if (v.x > 0.0) a = -a;

	glPushMatrix();
	glTranslatef(startPos.x, startPos.y, startPos.z);
	glRotatef(a, 0.0, 0.0, 1.0);
	float space = 0.25;
	glBegin(GL_LINES);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, length, 0.0);

	glVertex3f(0.0, length, 0.0);
	glVertex3f(-space, length - (space * 1.5), 0.0);

	glVertex3f(0.0, length, 0.0);
	glVertex3f(space, length - (space * 1.5), 0.0);
	glEnd();
	glPopMatrix();
}

/**
*	Draw blue horizontal and vertical dashed lines one unit long along the y and x axis.
*/
void drawBasis() {
	float dashAmount = 10;
	float dashLen = 0.1;

	//draw a blue horizontal line, one unit long
	glLineWidth(3.0);
	glColor3f(0.0, 0.0, 1.0);
	glPushMatrix();
		glTranslatef(0.0, 0.0, 0.0);
		glBegin(GL_LINES);
		for (int i = 0; i < dashAmount; i++) {
			if (i % 2 != 0) { //ODD
				glVertex2f(0.0 + i *dashLen, 0.0);
				glVertex2f(dashLen + i*dashLen, 0.0);
			}
		}		
		glEnd();
	glPopMatrix();

	//draw a blue vertical line, one unit high
	glLineWidth(3.0);
	glColor3f(0.0, 0.0, 1.0);
	glPushMatrix();
	glBegin(GL_LINES);
	for (int i = 0; i < dashAmount; i++) {
		if (i % 2 != 0) { //ODD
			glVertex2f(0.0, 0.0 + i *dashLen);
			glVertex2f(0.0, dashLen + i*dashLen);
		}
	}
	glEnd();
	glPopMatrix();
}

/**
*	Draw a big dot marking the origin
*/
void drawOrig() {
	glPointSize(8.0);
	glColor3f(1.0, 1.0, 1.0);
	glPushMatrix();
		glTranslatef(0.0, 0.0, 0.0);
		glBegin(GL_POINTS);
			glVertex2f(0.0, 0.0);
		glEnd();
	glPopMatrix();
}

/**
*	Translate the opengl modelview matrix manually using own matrix class
*/
void manualTranslation(float x, float y, float z) {
	MyMatrix v;
	glMatrixMode(GL_MODELVIEW_MATRIX);
	v.translateMatrix(x, y, z);
}

/**
*	Rotate the opengl modelview matrix manually using own matrix class
*/
void manualZrotation(int degrees) {
	MyMatrix v;
	glMatrixMode(GL_MODELVIEW_MATRIX);
	v.rotateZAxis(degrees);
}

/**
*	Draw two squares next to each other and have them rotate on a keypress
*	One of them rotate around one of its vertices rather than its center
*/
void task2(void) {
	glPushMatrix();
		manualTranslation(1.0, 1.0, -5.0);
		manualTranslation(-1.0, -1.0, 0.0);  //translate to bottom left corner
		manualZrotation(rotateAmount); //rotate..
		manualTranslation(1.0, 1.0, 0.0); //translate back to center of square
		draw_square();
		
	glPopMatrix();

	glPushMatrix();
		manualTranslation(-1.0, 1.0, -5.0);
		manualZrotation(rotateAmount);
		draw_square();
	glPopMatrix();
}

/**
*	Print out the given quats values
*/
void printQuat(MyQuat q) {
	std::cout << "W: " << q.w << " (X Y Z) " << q.v.x << " " << q.v.y << " " << q.v.z << std::endl;
}

/**
*	Print out the given positions xyz coords
*/
void printPos(MyPosition & p) {
	float len = sqrt((p.x * p.x) + (p.y * p.y) + (p.z * p.z));
	std::cout << "P: " << p.x << " " << p.y << " " << p.z << "  length: " << len << std::endl;
}

/**
*	Rotate the point (1,1,0) around the axis (0,0,1) 45 degrees using the quaternion class
*/
void task3dot2(void) {
	MyPosition p;
	p.x = 1.0; p.y = 1.0; p.z = 0.0;
	MyVector axis(0.0, 0.0, 1.0);
	MyQuat qvec(p);

	MyQuat q1(45, axis);

	MyQuat q1Conj = q1.getConjugate();

	MyQuat qrA = qvec.multiplyBy(q1Conj);

	MyQuat qr = q1.multiplyBy(qrA);
	MyVector res = qr.v;

	MyPosition orig;
	orig.x = 0.0; orig.y = 0.0; orig.z = 0.0;

	glPushMatrix();
		manualTranslation(-1.0, -1.0, -6.0);
		
		//Show initial point (vector)
		MyPosition initPoint;
		initPoint.x = p.x; initPoint.y = p.y; initPoint.z = p.z;
		glColor3f(1.0, 1.0, 0.0); //yellow

		DrawLine(orig, initPoint);

		//show final rotated point (vector)
		glColor3f(1.0, 0.0, 0.0); //Red

		MyPosition resPoint;
		resPoint.x = res.x; resPoint.y = res.y; resPoint.z = res.z;

		DrawLine(orig, resPoint);
		
		//show basis
		drawBasis();

		//Show orig (point) 
		drawOrig();

	glPopMatrix();	
}

/**
*	Rotate the point (0,-10,0) around the axis (10,0,0) 45 degrees using the quaternion class
*/
void task3dot3(void) {
	MyPosition p;
	p.x = 0.0; p.y = -10.0; p.z = 0.0;
	MyVector axis(10.0, 0.0, 0.0);
	MyQuat qvec(p);

	MyQuat q1(45, axis);

	MyQuat q1Conj = q1.getConjugate();

	MyQuat qrA = qvec.multiplyBy(q1Conj);

	MyQuat qr = q1.multiplyBy(qrA);
	MyVector res = qr.v;
	
	MyPosition orig;
	orig.x = 0.0; orig.y = 0.0; orig.z = 0.0;

	glPushMatrix();
		manualTranslation(1.0, 4.0, -15.0);

		
		//Show initial point (vector)
		MyPosition initPoint;
		initPoint.x = p.x; initPoint.y = p.y; initPoint.z = p.z;
		glColor3f(1.0, 1.0, 0.0); //yellow

		DrawLine(orig, initPoint);

		//show final rotated point (vector)
		glColor3f(1.0, 0.0, 0.0); //Red
		
		MyPosition resPoint;
		resPoint.x = res.x; resPoint.y = res.y; resPoint.z = res.z;

		DrawLine(orig, resPoint);

		//show basis
		drawBasis();

		//Show orig (point)
		drawOrig();

	glPopMatrix();
}

int main(int argc, char *argv[])
{
  //Initialise Glut and create a window
  glutInit(&argc, argv);
  //sets up our display mode
  //here we've selected an RGBA display with depth testing 
  //and double buffering enabled
  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
  //create a window and pass through the windows title
  glutCreateWindow("Basic Glut Application");

  //run our own drawing initialisation routine
  init_drawing();

  //tell glut the names of our callback functions point to our 
  //functions that we defined at the start of this file
  glutReshapeFunc(reshape);
  glutKeyboardFunc(key);
  glutIdleFunc(idle);
  glutDisplayFunc(draw);

  //request a window size of 600 x 600
  glutInitWindowSize(600,600);
  glutReshapeWindow(600,600);

  //go into the main loop
  //this loop won't terminate until the user exits the 
  //application
  glutMainLoop();

  return 0;
}

/**
*	Draw callback function - this is called by glut whenever the window needs to be redrawn
*/
void draw(void)
{
  //clear the current window
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  //make changes to the modelview matrix
  glMatrixMode(GL_MODELVIEW);
  //initialise the modelview matrix to the identity matrix
  glLoadIdentity();
  
  //***DO ALL YOUR DRAWING HERE****//
  //.........................ANSWERS.........................
  //task2();
  //task3dot2();
  task3dot3();

  //flush what we've drawn to the buffer
  glFlush();
  //swap the back buffer with the front buffer
  glutSwapBuffers();
}

/**
*	Idle callback function - this is called when there is nothing else to do
*/
void idle(void)
{
  //this is a good place to do animation

	//rotateAmount += rotateIncrement;
	//draw();
}

/**
*	Key callback function - called whenever the user presses a KEY
*/
void key(unsigned char k, int x, int y)
{
  switch(k)
  {
    case 27: //27 is the ASCII code for the ESCAPE key
      exit(0);
      break;
	case 114: //r-key
		rotateAmount += rotateIncrement;
		glutPostRedisplay();
	  break;
  }
}

/**
*	Reshape callback function - called when the window size changed
*/
void reshape(int width, int height)
{
  //set the viewport to be the same width and height as the window
  glViewport(0,0,width, height);
  //make changes to the projection matrix
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  //set up our projection type
  //we'll be using a perspective projection, with a 90 degree 
  //field of view
  gluPerspective(45.0, (float) width / (float) height, 1.0, 100.0);
  //redraw the view during resizing
  draw();
}

/**
*	Set up OpenGL before we do any drawing
*	This function is only called once at the start of the program
*/
void init_drawing(void)
{
  //blend colours across the surface of the polygons
  //another mode to try is GL_FLAT which is flat shading
  glShadeModel(GL_SMOOTH);
  //turn lighting off
  glDisable(GL_LIGHTING);
  //enable OpenGL hidden surface removal
  glDepthFunc(GL_LEQUAL);
  glEnable(GL_DEPTH_TEST);
}