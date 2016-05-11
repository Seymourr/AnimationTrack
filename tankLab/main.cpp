#define WIN32_LEAN_AND_MEAN

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(linker, "/subsystem:console")

#include "windows.h"

#include <gl/gl.h>            // standard OpenGL include
#include <gl/glu.h>           // OpenGL utilties
#include <glut.h>             // OpenGL utilties

#include "myvector.h"
#include "mymatrix.h"
#include "bSphere.h"
using namespace MyMathLibrary;

#include "stdlib.h"
#include "stdio.h"

#include "objloader.h"
#include <iostream>
#include <string>

ObjMesh* tankBody;
ObjMesh* tankTurret;
ObjMesh* tankMainGun;
ObjMesh* tankSecondaryGun;
ObjMesh* tankWheel;

void load_tank_objs(void);
void draw_wheel(float, float, float, bool);
void compile_tank(float, float, float);
void drawObj(ObjMesh *);
void createBoundingSpheres();
void potentialPenetration(float, float, float);
void printSphere(float, float, float, std::string, BoundingSphere);
void drawProjectile();
void drawPoint(BoundingSphere);
float getDist(float, float, float, BoundingSphere);

float zPos = -30.0;
float yRot = 0.0;
float wheelRot = 0.0;
float turretRot = 0.0;
float rotateMainGun = 0.0;
float rotateSecondGun = 0.0;
BoundingSphere bSpheres[5];
float projX = 0.0;
float projY = 10.0;
float projZ = 0.0;

int tankBodyID;
int tankTurretID;
int tankMainGunID;
int tankSecondGunID;
int tankWheelID;

//prototypes for our callback functions
void draw(void);    //our drawing routine
void idle(void);    //what to do when nothing is happening
void key(unsigned char k, int x, int y);  //handle key presses
void reshape(int width, int height);      //when the window is resized
void init_drawing(void);                  //drawing intialisation

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

  load_tank_objs();
  compile_tank(0.0, 0.0, 0.0);
  createBoundingSpheres();

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

void drawObj(ObjMesh * obj) {
	for (int i = 0; i < obj->m_iNumberOfFaces; ++i) {
		ObjFace * objF = &obj->m_aFaces[i];
		glBegin(GL_TRIANGLES);
		for (int j = 0; j < 3; ++j) { //3 vertices per face
			glTexCoord2f(obj->m_aTexCoordArray[objF->m_aTexCoordIndicies[j]].u, 
							obj->m_aTexCoordArray[objF->m_aTexCoordIndicies[j]].v);
			glNormal3f(obj->m_aNormalArray[objF->m_aNormalIndices[j]].x,
						obj->m_aNormalArray[objF->m_aNormalIndices[j]].y,
						obj->m_aNormalArray[objF->m_aNormalIndices[j]].z);
			glVertex3f(obj->m_aVertexArray[objF->m_aVertexIndices[j]].x,
				obj->m_aVertexArray[objF->m_aVertexIndices[j]].y,
				obj->m_aVertexArray[objF->m_aVertexIndices[j]].z);
		}
		glEnd();
	}
}

void load_tank_objs(void)
{
  tankBody = LoadOBJ(".\\tankobjs\\tankbody.obj");
	tankBodyID = glGenLists(1);
  tankTurret = LoadOBJ(".\\tankobjs\\tankturret.obj");
	tankTurretID = glGenLists(1);
  tankMainGun = LoadOBJ(".\\tankobjs\\tankmaingun.obj");
	tankMainGunID = glGenLists(1);
  tankSecondaryGun = LoadOBJ(".\\tankobjs\\tanksecondarygun.obj");
	tankSecondGunID = glGenLists(1);
  tankWheel = LoadOBJ(".\\tankobjs\\tankwheel.obj");
	tankWheelID = glGenLists(1);

  SetTextures(tankBody->m_iMeshID, NULL, ".\\tankobjs\\texture.tga");
}


void compile_tank(float x, float y, float z)
{
	glNewList(tankBodyID, GL_COMPILE);
		glTranslatef(x, y, z);
		glScalef(0.1, 0.1, 0.1);		//reduce the size of the tank on screen
		drawObj(tankBody);
	glEndList();

	glNewList(tankWheelID, GL_COMPILE);
		drawObj(tankWheel);
	glEndList();


	glNewList(tankTurretID, GL_COMPILE);
		drawObj(tankTurret);
	glEndList();


	glNewList(tankMainGunID, GL_COMPILE);
		drawObj(tankMainGun);
	glEndList();


	glNewList(tankSecondGunID, GL_COMPILE);
		drawObj(tankSecondaryGun);
	glEndList();
}


void draw_tank()
{
	
	glPushMatrix();

		glCallList(tankBodyID);

		//Side one
		draw_wheel(-24.0f, -10.0f, -57.0f, false);
		draw_wheel(-24.0f, -10.0f, -42.0f, false);
		draw_wheel(-24.0f, -10.0f, -27.0f, false);
		draw_wheel(-24.0f, -10.0f, -12.0f, false);
		draw_wheel(-24.0f, -10.0f, 3.0f, false);
		draw_wheel(-24.0f, -10.0f, 18.0f, false);
		draw_wheel(-24.0f, -10.0f, 40.0f, false);

		//Side two
		draw_wheel(23.0f, -10.0f, -57.0f, true);
		draw_wheel(23.0f, -10.0f, -42.0f, true);
		draw_wheel(23.0f, -10.0f, -27.0f, true);
		draw_wheel(23.0f, -10.0f, -12.0f, true);
		draw_wheel(23.0f, -10.0f, 3.0f, true);
		draw_wheel(23.0f, -10.0f, 18.0f, true);
		draw_wheel(23.0f, -10.0f, 40.0f, true);

		//Turret
		glPushMatrix();
			glTranslatef(0.0, 13.0, 0.0);
			glRotatef(turretRot, 0.0, 1.0, 0.0);
			glCallList(tankTurretID);

			//Main gun
			glPushMatrix();
				//Rotate beforehand to not do a massive rotation..
				glRotatef(-rotateMainGun, 1.0, 0.0, 0.0);
				glTranslatef(53.75, -102.0, 11.0);
				glCallList(tankMainGunID);
			glPopMatrix();

			//Secondary gun
			glPushMatrix();
				glTranslatef(-12.5, 15.0, -4.0);
				glRotatef(rotateSecondGun, 0.0, 1.0, 0.0);
				glCallList(tankSecondGunID);
			glPopMatrix();
		glPopMatrix();

		drawPoint(bSpheres[0]);
	glPopMatrix();
}

void draw_wheel(float x, float y, float z, bool rotate) {
	glPushMatrix();
		glTranslatef(x, y, z);
		if (rotate) {
			glRotatef(180, 0.0, 1.0, 0.0);
			glRotatef(-wheelRot, 1.0, 0.0, 0.0);
		} else {
			glRotatef(wheelRot, 1.0, 0.0, 0.0);
		}
		glCallList(tankWheelID);
	glPopMatrix();
}

void createBoundingSpheres() {
	BoundingSphere tankSphere = BoundingSphere();
	ObjMesh * tankMesh[4];
	tankMesh[0] = tankBody;
	tankMesh[1] = tankTurret;
	tankMesh[2] = tankMainGun;
	tankMesh[3] = tankSecondaryGun;
	//TODO: Wheels..?
	tankSphere.createBoundingSphere(tankMesh, 4);

	BoundingSphere tankBodySphere = BoundingSphere();
	ObjMesh * tankBodyMesh[1];
	tankBodyMesh[0] = tankBody;
	tankBodySphere.createBoundingSphere(tankBodyMesh, 1);

	BoundingSphere tankTurretSphere = BoundingSphere();
	ObjMesh * tankTurretMesh[1];
	tankTurretMesh[0] = tankTurret;
	tankTurretSphere.createBoundingSphere(tankTurretMesh, 1);

	BoundingSphere tankMainGunSphere = BoundingSphere();
	ObjMesh * tankMainGunMesh[1];
	tankMainGunMesh[0] = tankMainGun;
	tankMainGunSphere.createBoundingSphere(tankMainGunMesh, 1);

	BoundingSphere tankSecondGunSphere = BoundingSphere();
	ObjMesh * tankSecondGunMesh[1];
	tankSecondGunMesh[0] = tankSecondaryGun;
	tankSecondGunSphere.createBoundingSphere(tankSecondGunMesh, 1);

	bSpheres[0] = tankSphere;
	bSpheres[1] = tankBodySphere;
	bSpheres[2] = tankTurretSphere;
	bSpheres[3] = tankMainGunSphere;
	bSpheres[4] = tankSecondGunSphere;

}

/**
*	Check if a point, given by coordinates, penetrates this tank.
*	If such is the case, print some informaiton about the collision.
*/
void potentialPenetration(float x, float y, float z) {
	std::string names[4] = { "tank body", "tank turret", "tank main gun", "tank secondary gun" };
	//Penetrating tankSphere?
	float d = getDist(x, y, z, bSpheres[0]);
	if (d <= bSpheres[0].radius) {
		printSphere(x, y, z, "tank", bSpheres[0]);
		//Check subparts
		for(int i = 0; i < 4; ++i) {
			d = getDist(x, y, z, bSpheres[i + 1]);
			if (d <= bSpheres[i + 1].radius) {
				printSphere(x, y, z, names[i], bSpheres[i + 1]);
			}
		}
	}
}

/**
*	Return the distance between a point and a BoundingSphere
*/
float getDist(float x, float y, float z, BoundingSphere s) {
	return sqrt(pow(x - s.centerX, 2)
		+ pow(y - s.centerY, 2)
		+ pow(z - s.centerZ, 2));
}
/**
*	Print information about a point colliding with a Bounding Sphere
*/
void printSphere(float x, float y, float z, std::string sphere, BoundingSphere s) {
	std::cout << "Potential penetration on " << sphere << ". "
		<< "	Pos: (" << x << ", " << y << ", " << z << ")"
		<< std::endl;
	std::cout << "	Sphere center: "
		<< "(" << s.centerX  << ", " << s.centerY << ", " << s.centerZ << ")"
		<< std::endl;
	std::cout << "	Sphere radius: "
		<< s.radius
		<< std::endl;
}

/**
*	Draw a point, aka "projectile", at specified coordinates
*/
void drawProjectile() {
	glPushMatrix();
		glPointSize(15.0);
		glColor3f(0.0, 1.0, 0.0);
		glTranslatef(projX, projY, projZ);
		glBegin(GL_POINTS);
			glVertex2f(0.0, 0.0);
		glEnd();
		glColor3f(1.0, 1.0, 1.0);
	glPopMatrix();
}

/**
*	Draw a sphere at the coordinates given by the given Bounding Sphere object
*/
void drawPoint(BoundingSphere s) {
	glPushMatrix();
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		glColor4f(1, 0, 0, 0.3);
		glTranslatef(s.centerX, s.centerY,s.centerZ);
	
		gluSphere(gluNewQuadric(), s.radius, 100, 20);
	
		glColor3f(1, 1, 1);
	glPopMatrix();
}
//draw callback function - this is called by glut whenever the 
//window needs to be redrawn
void draw(void)
{
  //clear the current window
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  //make changes to the modelview matrix
  glMatrixMode(GL_MODELVIEW);
  //initialise the modelview matrix to the identity matrix
  glLoadIdentity();

  glTranslatef(0.0,-5.0,zPos);
  
  glRotatef(yRot,0.0,1.0,0.0);

  //draw a projectile on screen at a position
  drawProjectile();
  potentialPenetration(projX, projY, projZ);
  //draw the tank on screen at a position
  draw_tank();

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
    case 'h':
	    zPos--;
	    break;
	case 'y':
	    zPos++;
	    break;
	case 'r':
		yRot++;
		break;
	case 't':
		yRot--;
		break;
	case 'w':
		wheelRot += 5;
		break;
	case 's':
		wheelRot -= 5;
		break;
	case 'a':
		turretRot++;
		break;
	case 'd':
		turretRot--;
		break;
	case 'q':
		if (rotateMainGun < 10.0) rotateMainGun++; //Limit max height (due to mesh limits)
		break;
	case 'e':
		if (rotateMainGun > 0.0) rotateMainGun--;
		break;
	case 'z':
		if(rotateSecondGun < 45) rotateSecondGun +=2;
		break;
	case 'x':
		if (rotateSecondGun > -45) rotateSecondGun -= 2;
		break;
	case '8':
		projY++;
		break;
	case '2':
		projY--;
		break;
	case '6':
		projX++;
		break;
	case '4':
		projX--;
		break;
	case '9':
		projZ++;
		break;
	case '7':
		projZ--;
		break;
    case 27: //27 is the ASCII code for the ESCAPE key
      exit(0);
      break;
  }
  glutPostRedisplay();
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
  gluPerspective(45.0, (float) width / (float) height, 1.0, 100.0);
  //redraw the view during resizing
  draw();
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
  glDepthFunc(GL_LEQUAL);
  glEnable(GL_DEPTH_TEST);

  GLfloat specular[] = {0.2,0.2,0.2,1.0};
  GLfloat ambient[] = {1.0,1.0,1.0,1.0};
  GLfloat diffuse[] = {1.0,1.0,1.0,1.0};
  GLfloat position[] = {0.0,30.0,0.0,1.0};
  glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
  glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
  glLightfv(GL_LIGHT0, GL_POSITION, position);
  glEnable(GL_LIGHTING);

  GLfloat position1[] = {10.0,30.0,0.0,1.0};
  glLightfv(GL_LIGHT1, GL_SPECULAR, specular);
  glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
  glLightfv(GL_LIGHT1, GL_POSITION, position1);
  glEnable(GL_LIGHT1);
  glEnable(GL_LIGHTING);



  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_TEXTURE_2D);

}