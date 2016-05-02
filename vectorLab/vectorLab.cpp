#define WIN32_LEAN_AND_MEAN

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(linker, "/subsystem:console")

#include "windows.h"
#include "vector.h"
#include "glut.h"             // OpenGL utilties
#include <iostream>
#include <string>

using namespace MyOGLProg;

#include "stdlib.h"

//prototypes for our callback functions
void DisplayScene(void);    //our drawing routine
void idle(void);    //what to do when nothing is happening
void key(unsigned char k, int x, int y);  //handle key presses
void reshape(int width, int height);      //when the window is resized
void init_drawing(void);                  //drawing intialisation

int toDraw = 1;

void DrawVector(Position& startPos, Vector& v1)
{
	//draw the vector v1 starting from position startPos
	//this function will only work as long as the z coordinate for both positions is zero
	float length = sqrt((v1.x * v1.x) + (v1.y * v1.y) + (v1.z * v1.z));
	Vector v;
	if (length > 0.0){ v.x = v1.x/length; v.y = v1.y/length; v.z = v1.z/length; }
	else return;
	float d = (v.x * 0.0) + (v.y * 1.0) + (v.z * 0.0);
	float a = RAD2DEG(acos(d));
	if (v.x > 0.0) a = -a;

	glPushMatrix();
	glTranslatef(startPos.x, startPos.y, startPos.z);
	glRotatef(a,0.0,0.0,1.0);
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

void DrawDashed(Position& startPos, Vector& v1)
{
	//draw the vector v1 starting from position startPos
	//this function will only work as long as the z coordinate for both positions is zero
	float length = sqrt((v1.x * v1.x) + (v1.y * v1.y) + (v1.z * v1.z));
	Vector v;
	if (length > 0.0) { v.x = v1.x / length; v.y = v1.y / length; v.z = v1.z / length; }
	else return;
	float d = (v.x * 0.0) + (v.y * 1.0) + (v.z * 0.0);
	float a = RAD2DEG(acos(d));
	if (v.x > 0.0) a = -a;

	glPushMatrix();
	glTranslatef(startPos.x, startPos.y, startPos.z);
	glRotatef(a, 0.0, 0.0, 1.0);
	float dashAmount = 10;
	float dashLen = length/dashAmount; 
	glBegin(GL_LINES);
	
	
	for (int i = 0; i < dashAmount; i++) {
		if (i % 2 != 0) { //ODD
			glVertex3f(0.0, 0.0 + i * dashLen, 0.0);
			glVertex3f(0.0, dashLen + i*dashLen, 0.0);
		}
	}

	glEnd();
	glPopMatrix();
}

void Answer1() {
	Vector v(4.0, 2.0, 0.0);
	Position p;
	p.x = 1.0; p.y = 2.0; p.z = 0.0;
	DrawVector(p, v);
}


void Answer2() {
	Vector v1(4.0, 2.0, 0.0);
	Position p;
	p.x = 0.0; p.y = 0.0; p.z = 0.0;
	Vector v2(-2.0, 3.0, 0.0);
	Vector res = v1.addTo(v2);
	DrawVector(p, res);
	glColor3f(1.0, 1.0, 1.0);
	DrawVector(p, v1);
	DrawVector(p, v2);
}

void Answer3() {
	Vector v1(0.0, 1.0, 0.0);
	Vector v2(0.707, 0.707, 0.0);
	Position p;
	p.x = 0.0; p.y = 0.0; p.z = 0.0;

	float tmp = v1.getDotProduct(v2);
	tmp = tmp / v1.getMagnitude();
	tmp = tmp / v2.getMagnitude();
	float angle = RAD2DEG(acos(tmp));
	std::cout << "The angle between v1 and v2 is: " << angle << " degrees" << std::endl;

	DrawVector(p, v1);
	DrawVector(p, v2);
}

void Answer4() {
	Vector v1(4.0, 4.0, 0.0);
	Vector v2(-2.0, 3.0, 0.0);
	Position p;
	p.x = 0.0; p.y = 0.0; p.z = 0.0;


	float dotProduct = v1.getDotProduct(v2);
	std::cout << dotProduct << std::endl;
	std::string res= "";
	
	if (dotProduct >= 0.0) { 
		res = "the same";
	} else {
		res = "not the same";
	}

	std::cout << "The direction of v1 and v2 point in "<< res << " general direction" << std::endl;
	//Is general direction sought, or precise direction?
	DrawVector(p, v1);
	DrawVector(p, v2);
}


void Answer5() {
	Vector v1(5.0, 4.0, 0.0); 
	Vector v2(3.0, 9.0, 0.0); 
	Position p;
	p.x = 0.0; p.y = 0.0; p.z = 0.0;
	Position p2;
	p2.x = v1.x; p2.y = v1.y; p2.z = v1.z;
	
	Vector res = v2;
	res.normalise();
	float disA = res.getDotProduct(v1);

	res = v2;
	res.setMagnitude(disA);
	Position p3;
	p3.x = res.x; p3.y = res.y; p3.z = res.z;

	res = Vector(p2, p3);

	glColor3f(0.0, 1.0, 0.0);
	DrawVector(p, v1);
	glColor3f(1.0, 0.0, 0.0);

	DrawVector(p, v2);

	glColor3f(1.0, 1.0, 0.0);
	DrawDashed(p2, res);

	glPointSize(5.0);
	glColor3f(0.0, 1.0, 0.0);
	glPushMatrix();
		glTranslatef(p3.x, p3.y, p3.z);
		glBegin(GL_POINTS);
		glVertex2f(0.0, 0.0);
		glEnd();
	glPopMatrix();
}

void Answer6() {
	Vector v1(1.0, 8.0, 0.0);
	Vector v2(5.0, 4.0, 0.0);
	Vector v3(3.0, 0.0, 0.0);
	Vector v4(-6.0, 0.0, 0.0);
	Position p;
	p.x = 0.0; p.y = 0.0; p.z = 0.0;


	Position p2;
	p2.x = v1.x; p2.y = v1.y; p2.z = v1.z;

	Position p3;
	p3.x = v2.x; p3.y = v2.y; p3.z = v2.z;

	Position p4;
	p4.x = v3.x; p4.y = v3.y; p4.z = v3.z;

	Position p5;
	p5.x = v4.x; p5.y = v4.y; p5.z = v4.z;

	Vector l1(p3, p2);
	Vector l2(p4, p5);

	DrawVector(p, v1);
	DrawVector(p, v2);
	DrawVector(p, v3);
	DrawVector(p, v4);

	glColor3f(1.0, 1.0, 1.0);
	DrawVector(p3, l1);
	DrawVector(p4, l2);
	
	float res = l1.getDotProduct(l2);
	res = res / l1.getMagnitude();
	res = res / l2.getMagnitude();
	res = RAD2DEG(acos(res));
	std::cout << "The angle between the lines is " << res << " degrees" << std::endl;

}

void Answer7() {

	glPointSize(6.0);
	glColor3f(1.0, 1.0, 1.0);

	glPushMatrix();
		glTranslatef(0.0, 0.0, 0.0);
		glBegin(GL_POINTS);
			glVertex3f(8.0, 3.0, 0.0);
		glEnd();
	glPopMatrix();

	Position p; 
	p.x = 8.0; p.y = 3.0; p.z = 0.0;

	Position p1;
	p1.x = -2.5; p1.y = -2.0; p1.z = 0.0;

	Position p2;
	p2.x = 5.0; p2.y = -2.0; p2.z = 0.0;

	Vector l1(p1, p2); //a - b


	glColor3f(1.0, 1.0, 0.0);

	Vector l2(p1, p); // a - p

	float sqrdMag = pow(l1.getMagnitude(), 2.0);
	float dotPr = l1.getDotProduct(l2);
	float normDistance = dotPr / sqrdMag;
	Position res;
	res.x = p1.x + l1.x * normDistance;
	res.y = p1.y + l1.y * normDistance;
	res.z = 0.0;
	Vector closeRes(res, p);


	Vector line(p1, res);
	glColor3f(1.0, 0.0, 0.0);
	DrawDashed(p1, line);
	glColor3f(1.0, 1.0, 0.0);
	DrawDashed(res, closeRes); //Closest point from line

	glColor3f(1.0, 1.0, 1.0);

	glPushMatrix();
	glTranslatef(0.0, 0.0, 0.0);
	glBegin(GL_POINTS);
	glVertex3f(res.x, res.y, 0.0);
	glEnd();
	glPopMatrix();
}
//our main routine
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
  glutDisplayFunc(DisplayScene);

  //request a window size of 600 x 600
  glutInitWindowSize(600,600);
  glutReshapeWindow(600,600);

  //go into the main loop
  //this loop won't terminate until the user exits the 
  //application
  glutMainLoop();

  return 0;
}



/*****************************************************************************
 DisplayScene()

 The work of the application is done here. This is called by glut whenever the 
//window needs to be redrawn
*****************************************************************************/

void DisplayScene(void)
{
  //clear the current window
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  //make changes to the modelview matrix
  glMatrixMode(GL_MODELVIEW);
  //initialise the modelview matrix to the identity matrix
  glLoadIdentity();

  glTranslatef(-3.0,-3.0,-10.0);
  
  /*glColor3f(1.0,0.0,0.0);
  glBegin(GL_POINTS);
	glVertex3f(0.0,0.0,-5.0);
  glEnd();*/

  GLfloat yellow[4] = { 1.0f, 1.0f, 0.2f, 1.0f };
  GLfloat blue[4] = { 0.2f, 0.2f, 1.0f, 1.0f };
  GLfloat green[4] = { 0.2f, 1.0f, 0.2f, 1.0f };
Position origin;
origin.x = origin.y = origin.z = 0.0;

  //define a position and a vector
  Position p1;
  p1.x = 1.0; p1.y = 1.0; p1.z = 0.0;
  Vector v1(4.0, 4.0, 0.0);

  //draw the vector at position
  glDisable(GL_LINE_STIPPLE);
  glLineWidth(1.0);
  glColor3f(1.0,1.0,0.0);
 // DrawVector(p1,v1);

  switch (toDraw) {
  case 1: 
	  Answer1();
	  break;
  case 2:
	  Answer2();
	  break;
  case 3:
	  Answer3();
	  break;
  case 4:
	  Answer4();
	  break;
  case 5:
	  Answer5();
	  break;
  case 6:
	  Answer6();
	  break;
  case 7:
	  Answer7();
	  break;
  }

  //draw a red horizontal line, one unit long
  glLineWidth(3.0);
  glColor3f(1.0,0.0,0.0);
  glPushMatrix();
	  glTranslatef(0.0, 0.0, 0.0);
	  glBegin(GL_LINES);
		glVertex2f(0.0,0.0);
		glVertex2f(1.0,0.0);
	  glEnd();
  glPopMatrix();

  //draw a green vertical line, one unit high
  glLineWidth(3.0);
  glColor3f(0.0,0.0,1.0);
  glPushMatrix();
	  glBegin(GL_LINES);
		glVertex2f(0.0,0.0);
		glVertex2f(0.0,1.0);
	  glEnd();
  glPopMatrix();

  //draw a white point at the origin
  glPointSize(2.0);
  glColor3f(1.0,1.0,1.0);
  glPushMatrix();
	  glTranslatef(0.0, 0.0, 0.0);
	  glBegin(GL_POINTS);
		glVertex2f(0.0,0.0);
	  glEnd();
  glPopMatrix();

  //flush what we've drawn to the buffer
  glFlush();
  //swap the back buffer with the front buffer
  glutSwapBuffers();
}

//idle callback function - this is called when there is nothing 
//else to do
void idle(void)
{
  //this is a good place to do animation
  //since there are no animations in this test, we can leave 
  //idle() empty
}

//key callback function - called whenever the user presses a 
//key
void key(unsigned char k, int x, int y)
{
  switch(k)
  {
    case 27: //27 is the ASCII code for the ESCAPE key
     exit(0);
      break;
  }
  if (49 >= 1 && k <= 55) {
	toDraw = k - 48;

	glutPostRedisplay();

  }
}

//reshape callback function - called when the window size changed
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
  gluPerspective(90.0, (float) width / (float) height, 1.0, 100.0);
  //redraw the view during resizing
  DisplayScene();
}

//set up OpenGL before we do any drawing
//this function is only called once at the start of the program
void init_drawing(void)
{
  //blend colours across the surface of the polygons
  //another mode to try is GL_FLAT which is flat shading
  glShadeModel(GL_SMOOTH);
  //turn lighting off
  glDisable(GL_LIGHTING);
  //enable OpenGL hidden surface removal
  //glDepthFunc(GL_LEQUAL);
  //glEnable(GL_DEPTH_TEST);

  //glClearColor(1.0,0.0,0.0,0.0);
}

