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
#include "myQuat.h"
#include "bSphere.h"


#include "stdlib.h"
#include "stdio.h"

#include "objloader.h"
#include <iostream>
#include <string>
#include <vector>

ObjMesh* tankBody;
ObjMesh* tankTurret;
ObjMesh* tankMainGun;
ObjMesh* tankSecondaryGun;
ObjMesh* tankWheel;

void load_tank_objs(void);
void draw_wheel(float, float, float, bool);
void compile_tank();
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

//Constant tank properties

//Tank in general + tank body
const float TANK_INIT_POSX = 0.0;
const float TANK_INIT_POSY = 0.0;
const float TANK_INIT_POSZ = 0.0;
const float TANK_SCALE = 0.1;

//Tank turret
const float TANK_TURRET_TX = 0.0;
const float TANK_TURRET_TY = 13.0;
const float TANK_TURRET_TZ = 0.0;

const float TANK_TURRET_RX = 0.0;
const float TANK_TURRET_RY = 1.0;
const float TANK_TURRET_RZ = 0.0;

//Tank Main Gun
const float TANK_MAING_TX = 53.75;
const float TANK_MAING_TY = -102.0;
const float TANK_MAING_TZ = 11.0;

const float TANK_MAING_RX = 1.0;
const float TANK_MAING_RY = 0.0;
const float TANK_MAING_RZ = 0.0;

//Tank Secondary Gun
const float TANK_SECONDG_TX = -12.5;
const float TANK_SECONDG_TY = 15.0;
const float TANK_SECONDG_TZ = -4.0;

const float TANK_SECONDG_RX = 0.0;
const float TANK_SECONDG_RY = 1.0;
const float TANK_SECONDG_RZ = 0.0;



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
  compile_tank();


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


void compile_tank()
{
	glNewList(tankBodyID, GL_COMPILE);
		glTranslatef(TANK_INIT_POSX, TANK_INIT_POSY, TANK_INIT_POSZ);
		glScalef(TANK_SCALE, TANK_SCALE, TANK_SCALE);		//reduce the size of the tank on screen
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
			glTranslatef(TANK_TURRET_TX, TANK_TURRET_TY , TANK_TURRET_TZ);
			glRotatef(turretRot, TANK_TURRET_RX, TANK_TURRET_RY, TANK_TURRET_RZ);
			glCallList(tankTurretID);

			//Main gun
			glPushMatrix();
				//Rotate beforehand to not do a massive rotation..
				//Notice sign
				glRotatef(-rotateMainGun, TANK_MAING_RX, TANK_MAING_RY, TANK_MAING_RZ);
				glTranslatef(TANK_MAING_TX, TANK_MAING_TY, TANK_MAING_TZ);
				glCallList(tankMainGunID);
				
			glPopMatrix();

			//Secondary gun
			glPushMatrix();
				glTranslatef(TANK_SECONDG_TX, TANK_SECONDG_TY, TANK_SECONDG_TZ);
				glRotatef(rotateSecondGun, TANK_SECONDG_RX, TANK_SECONDG_RY, TANK_SECONDG_RZ);
				glCallList(tankSecondGunID);
		
			glPopMatrix();
		glPopMatrix();

	

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

void translateMesh(ObjMesh &m, float x, float y, float z, float scale) {
	for (int i = 0; i < m.m_iNumberOfVertices; ++i) {
		m.m_aVertexArray[i].x += x*scale;
		m.m_aVertexArray[i].y += y*scale;
		m.m_aVertexArray[i].z += z*scale;
	}
}

void rotateMesh(ObjMesh &m, float angle, float x, float y, float z, float scale) {
	MyVector axis(x*scale, y*scale, z*scale);
	MyQuat q(angle, axis);
	MyMatrix rotMat = q.convertToRotationMatrix();

	for (int i = 0; i < m.m_iNumberOfVertices; ++i) {
		float tX = m.m_aVertexArray[i].x;
		float tY = m.m_aVertexArray[i].y;
		float tZ = m.m_aVertexArray[i].z;

		float rX = rotMat[0] * tX + rotMat[1] * tY + rotMat[2] * tZ;
		float rY = rotMat[4] * tX + rotMat[5] * tY + rotMat[6] * tZ;
		float rZ = rotMat[8] * tX + rotMat[9] * tY + rotMat[10] * tZ;
		m.m_aVertexArray[i].x = rX;
		m.m_aVertexArray[i].y = rY;
		m.m_aVertexArray[i].z = rZ;
	}
}

void scaleMesh(ObjMesh &m, float s) {
	for (int i = 0; i < m.m_iNumberOfVertices; ++i) {
		m.m_aVertexArray[i].x *= s;
		m.m_aVertexArray[i].y *= s;
		m.m_aVertexArray[i].z *= s;
	}
}

std::vector<ObjVertex> saveVert(const ObjMesh m) {
	std::vector<ObjVertex> v;
	for (int i = 0; i < m.m_iNumberOfVertices; ++i) {
		ObjVertex t;
		t.x = m.m_aVertexArray[i].x;
		t.y = m.m_aVertexArray[i].y;
		t.z = m.m_aVertexArray[i].z;
		v.push_back(t);
	}
	return v;
}

void recoverVert(std::vector<ObjVertex> v, ObjMesh m) {
	for (int i = 0; i < m.m_iNumberOfVertices; ++i) {
		m.m_aVertexArray[i].x = v[i].x;
		m.m_aVertexArray[i].y = v[i].y;
		m.m_aVertexArray[i].z = v[i].z;
	}
}
void createBoundingSpheres() {
		ObjMesh tankMesh[4];
		
		std::vector<ObjVertex> v1 = saveVert(*tankBody);
		std::vector<ObjVertex> v2 = saveVert(*tankTurret);
		std::vector<ObjVertex> v3 = saveVert(*tankMainGun);
		std::vector<ObjVertex> v4 = saveVert(*tankSecondaryGun);

		tankMesh[0] = *tankBody;

		tankMesh[1] = *tankTurret;

		tankMesh[2] = *tankMainGun;

		tankMesh[3] = *tankSecondaryGun;


		for (int i = 0; i < 4; ++i) {
			translateMesh(tankMesh[i], TANK_INIT_POSX, TANK_INIT_POSY, TANK_INIT_POSZ, 1.0);
			scaleMesh(tankMesh[i], TANK_SCALE);
		}
		translateMesh(tankMesh[1], TANK_TURRET_TX, TANK_TURRET_TY, TANK_TURRET_TZ, TANK_SCALE);
		rotateMesh(tankMesh[1], turretRot, TANK_TURRET_RX, TANK_TURRET_RY, TANK_TURRET_RZ, TANK_SCALE);

		translateMesh(tankMesh[2], TANK_TURRET_TX, TANK_TURRET_TY, TANK_TURRET_TZ, TANK_SCALE);
		rotateMesh(tankMesh[2], turretRot, TANK_TURRET_RX, TANK_TURRET_RY, TANK_TURRET_RZ, TANK_SCALE);
		rotateMesh(tankMesh[2], -rotateMainGun, TANK_MAING_RX, TANK_MAING_RY, TANK_MAING_RZ, TANK_SCALE);
		translateMesh(tankMesh[2], TANK_MAING_TX, TANK_MAING_TY, TANK_MAING_TZ, TANK_SCALE);
	
		translateMesh(tankMesh[3], TANK_TURRET_TX, TANK_TURRET_TY, TANK_TURRET_TZ, TANK_SCALE);
		rotateMesh(tankMesh[3], turretRot, TANK_TURRET_RX, TANK_TURRET_RY, TANK_TURRET_RZ, TANK_SCALE);
		translateMesh(tankMesh[3], TANK_SECONDG_TX, TANK_SECONDG_TY, TANK_SECONDG_TZ, TANK_SCALE);
		rotateMesh(tankMesh[3], rotateSecondGun, TANK_SECONDG_RX, TANK_SECONDG_RY, TANK_SECONDG_RZ, TANK_SCALE);

		BoundingSphere tankSphere = BoundingSphere();
		
		
		tankSphere.createBoundingSphere(tankMesh, 4);

		BoundingSphere tankBodySphere = BoundingSphere();
		ObjMesh tankBodyMesh[1];
		tankBodyMesh[0] = tankMesh[0];
		tankBodySphere.createBoundingSphere(tankBodyMesh, 1);

		BoundingSphere tankTurretSphere = BoundingSphere();
		ObjMesh tankTurretMesh[1];
		tankTurretMesh[0] = tankMesh[1];
		tankTurretSphere.createBoundingSphere(tankTurretMesh, 1);

		BoundingSphere tankMainGunSphere = BoundingSphere();
		ObjMesh tankMainGunMesh[1];
		tankMainGunMesh[0] = tankMesh[2];
		tankMainGunSphere.createBoundingSphere(tankMainGunMesh, 1);

		BoundingSphere tankSecondGunSphere = BoundingSphere();
		ObjMesh tankSecondGunMesh[1];
		tankSecondGunMesh[0] = tankMesh[3];
		tankSecondGunSphere.createBoundingSphere(tankSecondGunMesh, 1);

		bSpheres[0] = tankSphere;
		bSpheres[1] = tankBodySphere;
		bSpheres[2] = tankTurretSphere;
		bSpheres[3] = tankMainGunSphere;
		bSpheres[4] = tankSecondGunSphere;

		recoverVert(v1, *tankBody);
		recoverVert(v2, *tankTurret);
		recoverVert(v3, *tankMainGun);
		recoverVert(v4, *tankSecondaryGun);
	
}

/**
*	Check if a point, given by coordinates, penetrates this tank.
*	If such is the case, print some informaiton about the collision.
*/
void potentialPenetration(float x, float y, float z) {
	std::string names[4] = { "tank body", "tank turret", "tank main gun", "tank secondary gun" };
	//Penetrating tankSphere?
	float d = getDist(x, y, z, bSpheres[0]);
	std::cout << d << std::endl;
	if (d <= bSpheres[0].radius) {
		std::cout << d << std::endl;
		std::cout << bSpheres[0].radius << std::endl;
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
		glColor4f(1, 0, 0, 0.4);
		glTranslatef(s.centerX, s.centerY, s.centerZ);
		gluSphere(gluNewQuadric(), s.radius, 100, 20);
		glColor3f(1, 1, 1);
	glPopMatrix();

	std::cout << "Sphere located at: (" << s.centerX << ", " << s.centerY << ", " << s.centerZ << ")" << std::endl;
	std::cout << "Radius: " << s.radius << std::endl;
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
  draw_tank();
  createBoundingSpheres();
  drawPoint(bSpheres[3]);
  potentialPenetration(projX, projY, projZ);
  //draw the tank on screen at a position


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