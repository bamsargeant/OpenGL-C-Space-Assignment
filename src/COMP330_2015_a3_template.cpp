/*
COMP330 - 2015

Assignment 3 Template.

Matt Cabanag
Tim Lambert
Scott McCallum
Nathan Tarr
+ others...

Note from Scott M: Matt deserves most of the credit for putting together this code template.
It is provided to you so you can "hit the ground running".
It is not meant to restrict your efforts in any way - that is, you can feel free to modify
anything you wish, or even start completely from scratch.
*/

#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <string>
#include <stdio.h>
#include <fstream>
#include <time.h>
#include <vector>
#include <GL/glut.h>
#include <GL/glu.h>
#include "Soil.h"
#include "UtilityStructs.h"
#include "Shapes.h"
#include "SceneObject.h"
#include "QuadCube.h"
#include "QuadSphere.h"
#include "Model.h"
#include "Mesh.h"

using namespace std;

#define X_RES 1024
#define Y_RES 768
#define MVP_X_RES 1024
#define MVP_Y_RES 600

//scene hierarchy root used for drawing
vector <SceneObject*> SceneRoot;

//intermediate scene data
vector <RawSceneData> MyRawData;

//movement speed variable for general use.
GLfloat moveSpeed = 0.25;

//variables for the main camera's location
GLfloat mainCamX = 0;
GLfloat mainCamY = 0;
GLfloat mainCamZ = 35;

//coordinates for what the camera is looking at
GLfloat mainCamLookX = 0;
GLfloat mainCamLookY = 0;
GLfloat mainCamLookZ = 0;

//variables for the main camera's "up" vector
GLfloat mainCamUpX = 0;
GLfloat mainCamUpY = 1.0;
GLfloat mainCamUpZ = 0;

//variables for the spaceship and light source
Mesh* mySpaceShip;

Vector3 shipPos;
Vector3 lightDir;

Colour4 lightAmbient;
Colour4 lightDiffuse;
Colour4 lightSpecular;

float cameraPitch = -10;
float cameraYaw = 25;
float cameraRadius = 35;
float camSpeed = 1;

float pitch = 0;
float yaw = 0;
float timeScale = 1.0f;

bool descent = false;

enum CAM_STATE{
	PERSPECTIVE = 0,
	TOP = 1,
	BRIDGE = 2,
	EXTERNAL = 3,
};

int mainCam = PERSPECTIVE;

//variables for texturing;
#define MAX_NO_TEXTURES 1
GLuint texture[MAX_NO_TEXTURES];

//Taken from Tim Lambert's "textures" example project, 2013.
int loadGLTexture(const char *fileName)                                    // Load Bitmaps And Convert To Textures
{
	glEnable(GL_TEXTURE_2D);
	string name = "textures/";
	name += fileName;
	/* load an image file from texture directory as a new OpenGL texture */
	texture[0] = SOIL_load_OGL_texture
			(
					name.c_str(),
					SOIL_LOAD_AUTO,
					SOIL_CREATE_NEW_ID,
					SOIL_FLAG_INVERT_Y
			);

	if(texture[0] == 0) {
		std::cerr << fileName << " : " << SOIL_last_result();
		return false;
	}

	// Typical Texture Generation Using Data From The Bitmap
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	return true;// Return Success
	glDisable(GL_TEXTURE_2D);
}

// Axes drawing function
// credit: Scott McCallum 2012.
// modified by Matt Cabanag 2014
void axes(void)
{
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glBegin(GL_LINES);
		  glLineWidth(4.0f);
		  glColor3f(1,0,0);
		  glVertex3f(0.0, 0.0, 0.0);
		  glVertex3f(50.0, 0.0, 0.0); /* draw x-axis */

		  glColor3f(0,1,0);
		  glVertex3f(0.0, 0.0, 0.0);
		  glVertex3f(0.0, 50.0, 0.0); /* draw y-axis */

		  glColor3f(0,0,1);
		  glVertex3f(0.0, 0.0, 0.0);
		  glVertex3f(0.0, 0.0, 50.0); /* draw z-axis */

		  glColor3f(1,1,1);
	glEnd();
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}

//sets up the main view
void setupMainView()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90, (float)MVP_X_RES/(float)MVP_Y_RES, 0.1, 10000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	switch (mainCam) {
		case PERSPECTIVE:
			//convert to radians
			pitch = cameraPitch * M_PI / 180;
			yaw = cameraYaw * M_PI / 180;

			//calculate camera position based on radius, pitch and yaw
			mainCamY = (0 * cos(pitch)) + (cameraRadius * -sin(pitch));
			mainCamZ = (0 * sin(pitch)) + (cameraRadius * cos(pitch));

			mainCamX = (0 * cos(yaw)) + (mainCamZ * sin(yaw));
			mainCamZ = (0 * -sin(yaw)) + (mainCamZ * cos(yaw));

			gluLookAt(mainCamX, mainCamY, mainCamZ,
					  mainCamLookX, mainCamLookY, mainCamLookZ,
					  mainCamUpX, mainCamUpY, mainCamUpZ);
			break;

		case TOP:
			gluLookAt(0, 60, 0,
					  0, 0, 0,
					  0, 0, 1);
			break;

		case BRIDGE:
			float lookAtX;
			float lookAtY;
			float lookAtZ;
			for (unsigned int i = 0; i < SceneRoot.size(); ++i) {
				if(SceneRoot[i]->name == "Sun"){
					for (unsigned int j = 0; j < SceneRoot[i]->getChildren().size(); ++j) {
						vector<SceneObject*> children = SceneRoot[i]->getChildren();
						if( children[j]->name == "Mars"){
							lookAtX = children[j]->absPos.x;
							lookAtY = children[j]->absPos.y;
							lookAtZ = children[j]->absPos.z;
						}
					}
				}
			}

			mainCamX = mySpaceShip->absPos.x;
			mainCamY = mySpaceShip->absPos.y;
			mainCamZ = mySpaceShip->absPos.z;

			gluLookAt(mainCamX, mainCamY, mainCamZ,
					  lookAtX, lookAtY, lookAtZ,
					  mainCamUpX, mainCamUpY, mainCamUpZ);
			break;

		case EXTERNAL:
			lookAtX;
			lookAtY;
			lookAtZ;
			for (unsigned int i = 0; i < SceneRoot.size(); ++i) {
				if(SceneRoot[i]->name == "Sun"){
					for (unsigned int j = 0; j < SceneRoot[i]->getChildren().size(); ++j) {
						vector<SceneObject*> children = SceneRoot[i]->getChildren();
						if( children[j]->name == "Mars"){
							lookAtX = children[j]->absPos.x;
							lookAtY = children[j]->absPos.y;
							lookAtZ = children[j]->absPos.z;
						}
					}
				}
			}

			mainCamX = mySpaceShip->absPos.x;
			mainCamY = mySpaceShip->absPos.y;
			mainCamZ = mySpaceShip->absPos.z;

			gluLookAt(mainCamX -1, mainCamY +0.5, mainCamZ,
					  mainCamX +1, mainCamY, mainCamZ,
					  mainCamUpX, mainCamUpY, mainCamUpZ);

		default:
			break;
	}



	//setup the depth buffer
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glEnable( GL_DEPTH_TEST );

	//have a light source at the origin!
	float lp [] = {1,1,1,1};
	glLightfv(GL_LIGHT0, GL_POSITION, lp);

	glEnable(GL_LIGHTING);// Enable lighting calculations
	glEnable(GL_LIGHT0);// Turn on light #0.
}

//sets up the top down view
void setupLeftView()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90,(float)(X_RES/3) / (float)(Y_RES-MVP_Y_RES), 0.1, 10000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	switch (mainCam) {
		case TOP:
			//convert to radians
			pitch = cameraPitch * M_PI / 180;
			yaw = cameraYaw * M_PI / 180;

			//calculate camera position based on radius, pitch and yaw
			mainCamY = (0 * cos(pitch)) + (cameraRadius * -sin(pitch));
			mainCamZ = (0 * sin(pitch)) + (cameraRadius * cos(pitch));

			mainCamX = (0 * cos(yaw)) + (mainCamZ * sin(yaw));
			mainCamZ = (0 * -sin(yaw)) + (mainCamZ * cos(yaw));

			gluLookAt(mainCamX, mainCamY, mainCamZ,
					  mainCamLookX, mainCamLookY, mainCamLookZ,
					  mainCamUpX, mainCamUpY, mainCamUpZ);
			break;

		default:
			gluLookAt(0, 60, 0,
					  0, 0, 0,
					  0, 0, 1);
			break;
	}


	//setup the depth buffer
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glEnable( GL_DEPTH_TEST );

	//have a light source at the origin!
	float lp [] = {0.0,1,0.0, 0.0};
	glLightfv(GL_LIGHT0, GL_POSITION, lp);

	glEnable(GL_LIGHTING);// Enable lighting calculations
	glEnable(GL_LIGHT0);// Turn on light #0.
}

//setup the bridge view (inside the ship)
void setupMidView()
{
	glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(90, (float)(X_RES/3) / (float)(Y_RES-MVP_Y_RES), 0.1, 10000);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		switch (mainCam) {
			case BRIDGE:
				//convert to radians
				pitch = cameraPitch * M_PI / 180;
				yaw = cameraYaw * M_PI / 180;

				//calculate camera position based on radius, pitch and yaw
				mainCamY = (0 * cos(pitch)) + (cameraRadius * -sin(pitch));
				mainCamZ = (0 * sin(pitch)) + (cameraRadius * cos(pitch));

				mainCamX = (0 * cos(yaw)) + (mainCamZ * sin(yaw));
				mainCamZ = (0 * -sin(yaw)) + (mainCamZ * cos(yaw));

				gluLookAt(mainCamX, mainCamY, mainCamZ,
						  mainCamLookX, mainCamLookY, mainCamLookZ,
						  mainCamUpX, mainCamUpY, mainCamUpZ);
				break;

			default:
				float lookAtX;
				float lookAtY;
				float lookAtZ;
				for (unsigned int i = 0; i < SceneRoot.size(); ++i) {
					if(SceneRoot[i]->name == "Sun"){
						for (unsigned int j = 0; j < SceneRoot[i]->getChildren().size(); ++j) {
							vector<SceneObject*> children = SceneRoot[i]->getChildren();
							if( children[j]->name == "Mars"){
								lookAtX = children[j]->absPos.x;
								lookAtY = children[j]->absPos.y;
								lookAtZ = children[j]->absPos.z;
							}
						}
					}
				}

				mainCamX = mySpaceShip->absPos.x;
				mainCamY = mySpaceShip->absPos.y;
				mainCamZ = mySpaceShip->absPos.z;

				gluLookAt(mainCamX, mainCamY, mainCamZ,
						  lookAtX, lookAtY, lookAtZ,
						  mainCamUpX, mainCamUpY, mainCamUpZ);
				break;
		}

		//setup the depth buffer
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
		glEnable( GL_DEPTH_TEST );

		//have a light source at the origin!
		float lp [] = {0.0,1,0.0, 0.0};
		glLightfv(GL_LIGHT0, GL_POSITION, lp);

		glEnable(GL_LIGHTING);// Enable lighting calculations
		glEnable(GL_LIGHT0);// Turn on light #0.
}

//setup the 3rd person view of the ship
void setupRightView()
{
	glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(90, (float)(X_RES/3) / (float)(Y_RES-MVP_Y_RES), 0.1, 10000);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		switch (mainCam) {
			case EXTERNAL:
				//convert to radians
				pitch = cameraPitch * M_PI / 180;
				yaw = cameraYaw * M_PI / 180;

				//calculate camera position based on radius, pitch and yaw
				mainCamY = (0 * cos(pitch)) + (cameraRadius * -sin(pitch));
				mainCamZ = (0 * sin(pitch)) + (cameraRadius * cos(pitch));

				mainCamX = (0 * cos(yaw)) + (mainCamZ * sin(yaw));
				mainCamZ = (0 * -sin(yaw)) + (mainCamZ * cos(yaw));

				gluLookAt(mainCamX, mainCamY, mainCamZ,
						  mainCamLookX, mainCamLookY, mainCamLookZ,
						  mainCamUpX, mainCamUpY, mainCamUpZ);
				break;

			default:
				float lookAtX;
				float lookAtY;
				float lookAtZ;
				for (unsigned int i = 0; i < SceneRoot.size(); ++i) {
					if(SceneRoot[i]->name == "Sun"){
						for (unsigned int j = 0; j < SceneRoot[i]->getChildren().size(); ++j) {
							vector<SceneObject*> children = SceneRoot[i]->getChildren();
							if( children[j]->name == "Mars"){
								lookAtX = children[j]->absPos.x;
								lookAtY = children[j]->absPos.y;
								lookAtZ = children[j]->absPos.z;
							}
						}
					}
				}

				mainCamX = mySpaceShip->absPos.x;
				mainCamY = mySpaceShip->absPos.y;
				mainCamZ = mySpaceShip->absPos.z;

				gluLookAt(mainCamX -1, mainCamY +0.5, mainCamZ,
						  mainCamX +1, mainCamY, mainCamZ,
						  mainCamUpX, mainCamUpY, mainCamUpZ);
				break;
		}


		//setup the depth buffer
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
		glEnable( GL_DEPTH_TEST );

		//have a light source at the origin!
		float lp [] = {0.0,1,0.0, 0.0};
		glLightfv(GL_LIGHT0, GL_POSITION, lp);

		glEnable(GL_LIGHTING);// Enable lighting calculations
		glEnable(GL_LIGHT0);// Turn on light #0.
}


//THE SCENE DRAWER!
void drawScene()
{
	//SceneRoot.at(0)->drawPath();
	for(unsigned int i = 0; i < SceneRoot.size(); i++)
	{
		SceneRoot.at(i) -> Display();
		SceneRoot.at(i) -> drawPath();
	}

}


void drawLaunchPhase()
{
	int a = mySpaceShip->flightState;
	string text;
	if(a == mySpaceShip->ReadyToLaunch)
		text = "Ready to Launch";
	else if(a == mySpaceShip->FlyingToMars)
		text = "Flying to Destination";
	else if(a == mySpaceShip->ReadyToDescend)
		text = "Ready to Descend";
	else if(a == mySpaceShip->LandedOnMars)
		text = "Successful Landing";

	glColor3f(1,1,1);
	glRasterPos2f(0,170);
	for (unsigned int i = 0; i < text.length(); i++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]);
	}


	int favCount = 0;
	for (unsigned int i = 0; i < SceneRoot.size(); ++i) {
		if(SceneRoot[i]->name == "Sun")
		{
			for (unsigned int j = 0; j < SceneRoot[i]->getChildren().size(); ++j) {
				if(SceneRoot[i]->getChildren()[j]->name == "Earth"){
					favCount += SceneRoot[i]->getChildren()[j]->getAngle();
//					if(SceneRoot[i]->getChildren()[j]->favourable)
//						favCount += 1;
//					else
//						favCount = 0;
				}
				if(SceneRoot[i]->getChildren()[j]->name == "Mars"){
//					favCount -= SceneRoot[i]->getChildren()[j]->getAngle();

//					if(SceneRoot[i]->getChildren()[j]->favourable)
//						favCount += 1;
//					else
//						favCount = 0;
				}
			}
		}
	}

//	favCount = 2;
	favCount = sqrtf(favCount*favCount);
//	cout << favCount << endl;

	if(favCount == 2)
	{
		text = "Favorable";
		glColor3f(1,1,1);
		glRasterPos2f(900,170);
		for (unsigned int i = 0; i < text.length(); i++) {
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]);
		}
	}
}

void drawVBorders(float x, float y, float w, float h)
{
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHT0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(x, w, y, h);
	glLineWidth(2);
	glColor3f(0.7,0.7,0.7);
	glBegin(GL_LINE_LOOP);
			glVertex2f(x, y);
			glVertex2f(w, y);
			glVertex2f(w, h);
			glVertex2f(x, h );
	glEnd();
	drawLaunchPhase();

	glLineWidth(1);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}

void launchShip()
{
	Vector3 shipRPY;
	float shipRadius;
	float shipSpeed;
	float shipYaw;

	vector<SceneObject*>children;
	for(unsigned int i = 0; i < SceneRoot.size(); i++)
	{
		if(SceneRoot.at(i)->name == "Sun"){
				for (unsigned int var = 0; var < SceneRoot.at(i)->getChildren().size(); ++var) {
					if(SceneRoot.at(i)->getChildren().at(var)->name == "Earth"){
						children = SceneRoot.at(i)->getChildren().at(var)->getChildren();
						int count = 0;
						for (unsigned int j = 0; j < children.size(); ++j) {
							if(children.at(j)->name == "Ship"){
								shipPos = children.at(j)->absPos;
								shipRPY = children.at(j)->rpy;
//								shipYaw = children.at(j)->objYaw;
//								cout << "shipyaw: " << shipYaw << endl;
								count += 1;
							}
						}
						if(count == 0)
							return;
						shipRadius = SceneRoot.at(i)->getChildren().at(var)->objRadius;
						shipSpeed =  SceneRoot.at(i)->getChildren().at(var)->rotSpeed;
						shipYaw = SceneRoot.at(i)->getChildren().at(var)->objYaw;
						cout << shipYaw << endl;
						SceneRoot.at(i)->getChildren().at(var)->removeChild(mySpaceShip);
					}
					if(SceneRoot.at(i)->getChildren().at(var)->name == "Mars"){
						mySpaceShip->destination = SceneRoot.at(i)->getChildren().at(var)->absPos;
					}
				}

				SceneRoot.at(i)->AddChild(mySpaceShip);
				mySpaceShip->position = shipPos;
				mySpaceShip->rpy = shipRPY;

				mySpaceShip->rotSpeed = shipSpeed;

				// this get's the earth's yaw and assigns it to the ships. Needs to add on the ship's yaw from the earth though...
				mySpaceShip->objYaw = shipYaw;

				mySpaceShip->rpySpeed = Vector3(0,0,0);
				mySpaceShip->launched = true;
				mySpaceShip->flightState = mySpaceShip->FlyingToMars;
//				cout <<// shipPos.x << ", " << shipPos.y << ", " << shipPos.z << ", " <<
//						shipYaw << "Launched" << mySpaceShip->launched <<endl;
		}
	}
}

void startDescent()
{
	if(mySpaceShip->objRadius > 1)
		mySpaceShip->objRadius -= 0.05 * timeScale;
	else{
		descent = false;
		mySpaceShip->flightState = mySpaceShip->LandedOnMars;
	}
}


//THE ANIMATOR!!
//called by the glut idle function.
void animateScene()
{
	for(unsigned int i = 0; i < SceneRoot.size(); i++)
	{
		SceneRoot.at(i)->Update();
		SceneRoot.at(i)->getChildAbsPos(Vector3(0,0,0));

		for(unsigned int i = 0; i < SceneRoot.size(); i++)
		{
			if(SceneRoot.at(i)->name == "Sun"){
				for (unsigned int var = 0; var < SceneRoot.at(i)->getChildren().size(); ++var) {
					if(SceneRoot.at(i)->getChildren().at(var)->name == "Mars"){
						mySpaceShip->destination = SceneRoot.at(i)->getChildren().at(var)->absPos;
						if(mySpaceShip->destReached){
							Vector3 shipRPY;
							float shipRadius;
							float shipSpeed;
							float shipYaw;

							shipRPY = mySpaceShip->rpy;
							shipSpeed = mySpaceShip->rotSpeed;
							shipYaw = mySpaceShip->objYaw;
							shipRadius = mySpaceShip->objRadius;

							SceneRoot.at(i)->removeChild(mySpaceShip);
							SceneRoot.at(i)->getChildren().at(var)->AddChild(mySpaceShip);

							mySpaceShip->rpy = shipRPY;
//							mySpaceShip->rotSpeed = shipSpeed;
							mySpaceShip->rotSpeed = SceneRoot.at(i)->getChildren().at(var)->rpySpeed.z;
							mySpaceShip->objYaw = shipYaw;
//							mySpaceShip->objRadius = shipRadius;
							mySpaceShip->objRadius = hypot((SceneRoot.at(i)->getChildren().at(var)->absPos.x - mySpaceShip->absPos.x),
														   (SceneRoot.at(i)->getChildren().at(var)->absPos.z - mySpaceShip->absPos.z));

							mySpaceShip->destReached = false;
							mySpaceShip->flightState = mySpaceShip->ReadyToDescend;
						}
					}
				}
			}
		}
	}
}

void animateSceneTimer(int milis)
{
	animateScene();
	glutTimerFunc(milis, animateSceneTimer, milis);
}

//the actual display callback function
void display(void)
{	//clear the screen!
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
//	glCullFace(GL_BACK);
	//remove all the polygons that will not be in view of the camera
	glEnable(GL_DEPTH_BUFFER_BIT);
	//first update the "logical model"; animate the world in other words
	//animateScene();

	drawLaunchPhase();

	//draw the main viewport
	setupMainView();
	glViewport(0, Y_RES-MVP_Y_RES, MVP_X_RES, MVP_Y_RES);
	//draw the axis:
	axes();

	if(mainCam == BRIDGE)
		mySpaceShip->cullAll = true;

	drawScene();
	drawVBorders(0, Y_RES-MVP_Y_RES, MVP_X_RES, MVP_Y_RES);
	drawLaunchPhase();
	mySpaceShip->cullAll = false;



	setupLeftView();
	glViewport(0, 0, X_RES/3, Y_RES-MVP_Y_RES);
	axes();
	drawScene();
	drawVBorders(0, 0, X_RES/3, Y_RES-MVP_Y_RES);

	if(mainCam != BRIDGE)
		mySpaceShip->cullAll = true;
	setupMidView();
	glViewport(X_RES/3 +0.3, 0, X_RES/3, Y_RES-MVP_Y_RES);
	axes();
	drawScene();
	drawVBorders(X_RES/3 +0.3, 0, X_RES/3, Y_RES-MVP_Y_RES);
	mySpaceShip->cullAll = false;


//	if(mainCam == PERSPECTIVE)
//		mySpaceShip->cullAll = true;
	setupRightView();
	glViewport(X_RES - X_RES/3 -0.3, 0, X_RES/3 +1, Y_RES-MVP_Y_RES);
	axes();
	drawScene();
	drawVBorders(X_RES - X_RES/3 -0.3, 0, X_RES/3 +1, Y_RES-MVP_Y_RES);
//	mySpaceShip->cullAll = false;

	if(descent){
		startDescent();
	}

	//OpenGL house cleaning
	glFlush();//put everything on the pipeline
	glutPostRedisplay();//post the ready scenes.
	glutSwapBuffers();//swap the buffers
}

//keyboard handler
void keyboard( unsigned char key, int x, int y )
{
	switch ( key )
	{	//move the view around with WASD
		case 'w':
		{
			cameraPitch -= 0.1 * camSpeed;
		}break;

		case 's':
		{
			cameraPitch += 0.1 * camSpeed;
		}break;

		case 'a':
		{
			cameraYaw -= 0.1 * camSpeed;
		}break;

		case 'd':
		{
			cameraYaw += 0.1 * camSpeed;
		}break;

		case 'q':
		{
			cameraRadius -= 0.1 * camSpeed;
		}break;

		case 'e':
		{
			cameraRadius += 0.1 * camSpeed;
		}break;

		//reset everything!
		case 'r':
		{
			cameraPitch = -10;
			cameraYaw = 25;
			cameraRadius = 35;
			timeScale = 1;
			for (unsigned int i = 0; i < SceneRoot.size(); ++i) {
				SceneRoot[i]->setTimeScale(timeScale);
			}
			mainCam = PERSPECTIVE;
		}break;

//		case '.':
//		{
//			shipPos.y += moveSpeed;
//		}break;
//
//		case ',':
//		{
//			shipPos.y -= moveSpeed;
//		}break;

		//this is the escape key
		case 27:
		{	exit(1);
		}break;

		//= button
		case 61:
		{
			if(camSpeed <= 20)
				camSpeed += 1;
		}break;

		//- button
		case 45:
		{
			if(camSpeed >= 0)
				camSpeed -= 1;
		}break;

		//shift _ button
		case 95:
		{
			if(timeScale >= 0)
			{
				timeScale -= 1;
				for (unsigned int i = 0; i < SceneRoot.size(); ++i) {
					SceneRoot[i]->setTimeScale(timeScale);
				}
			}

		}break;

		//shift + button
		case 43:
		{
			if(timeScale < 10)
			{
				timeScale += 1;
				for (unsigned int i = 0; i < SceneRoot.size(); ++i) {
					SceneRoot[i]->setTimeScale(timeScale);
				}
			}
		}break;

		case '1':
		{
			mainCam = PERSPECTIVE;
		}break;

		case '2':
		{
			mainCam = TOP;
		}break;

		case '3':
		{
			mainCam = BRIDGE;
		}break;

		case '4':
		{
			mainCam = EXTERNAL;
		}break;


		case ' ':
		{
			if(mySpaceShip->flightState == mySpaceShip->ReadyToLaunch)
			{
				launchShip();
			}
			if(mySpaceShip->flightState == mySpaceShip->ReadyToDescend)
			{
				descent = true;
			}

		}break;

	}

//	mySpaceShip -> position = shipPos;

	display();
}

//
//void SpecialInput(int key, int x, int y)
//{
//	switch(key)
//	{
//		case GLUT_KEY_UP:
//		{
//			shipPos.z -= moveSpeed;
//		}break;
//
//		case GLUT_KEY_DOWN:
//		{
//			shipPos.z += moveSpeed;
//		}break;
//
//		case GLUT_KEY_LEFT:
//		{
//			shipPos.x -= moveSpeed;
//		}break;
//
//		case GLUT_KEY_RIGHT:
//		{
//			shipPos.x += moveSpeed;
//		}break;
//
//		case GLUT_KEY_F1:
//		{
//
//		}break;
//	}
//
////	mySpaceShip -> position = shipPos;
//
//	display();
//}

void LoadDefaultScene()
{
	//if these are the first textures loaded into the program
	loadGLTexture("sun_surface.jpg");//textureID 1
	loadGLTexture("Earth.jpg");//textureID 2
	loadGLTexture("stars.png");//textureID 3
	loadGLTexture("moon.jpg");//textureID 4
	loadGLTexture("mars.jpg");//textureID 5
	loadGLTexture("red.jpg");//textureID 6

	// Set the time scale - the rotation of the solar system for all elements inside
	// Is bound to each element at end of function
	timeScale = 1;

	//Skybox
	QuadSphere* sky = new QuadSphere;
	sky->name = "Sky";
	sky->textureID = 3;
	sky->scale = Vector3(100,100,100);
	sky->lighting = false;


	//Sun
	QuadSphere * sun = new QuadSphere;
	sun = (new QuadSphere);

	sun->colour = Colour4(1,1,1,1);
	sun->textureID = 1;
	//Flip the sun sideways
	sun->rpy = Vector3(-90,0,0);
	sun->rpySpeed = Vector3(0,0,0.15f);
	sun->scale = Vector3(3, 3, 3);
	sun->name = "Sun";
	sun->lighting = false;

	//Earth
	QuadSphere * earth = new QuadSphere;
	earth->colour = Colour4(1,1,1,1);
	earth->position = Vector3(0,0,20);
	//notice having to flip the sphere sideways to
	earth->rpy = Vector3(-90,0,0);
	earth->rpySpeed = Vector3(0,0,0.9);
	earth->textureID = 2;
	earth->scale = Vector3(0.75, 0.75, 0.75);
	earth->objRadius = 20;
	earth->rotSpeed = 0.67/15;
	earth->name = "Earth";

	//Mars
	QuadSphere * mars = new QuadSphere;
	mars->colour = Colour4(1,1,1,1);
	mars->position = Vector3(0,0,32);
	//notice having to flip the sphere sideways to
	mars->rpy = Vector3(-90,0,0);
	mars->rpySpeed = Vector3(0,0,0.3);
	mars->textureID = 5;
	mars->scale = Vector3(0.5, 0.5, 0.5);
	mars->objRadius = 32;
	mars->rotSpeed = 0.53/15;
	mars->name = "Mars";

	//Moon
	QuadSphere * moon = new QuadSphere;
	moon->colour = Colour4(1,1,1,1);
	moon->position = Vector3(0,0,0.3);
	//notice having to flip the sphere sideways to
	moon->rpy = Vector3(-90,0,0);
	moon->rpySpeed = Vector3(0,0,0.5);
	moon->scale = Vector3(0.25,0.25,0.25);
	moon->textureID = 4;
	moon->objRadius = 3;
	moon->rotSpeed = 2;
	moon->name = "Moon";

	//marsMoon1
	QuadSphere * marsMoon1 = new QuadSphere;
	marsMoon1->colour = Colour4(1,1,1,1);
	marsMoon1->position = Vector3(0,0,0.3);
	//notice having to flip the sphere sideways to
	marsMoon1->rpy = Vector3(-90,0,0);
	marsMoon1->rpySpeed = Vector3(0,0,0.4);
	marsMoon1->scale = Vector3(0.02,0.02,0.02);
	marsMoon1->textureID = 4;
	marsMoon1->objRadius = 2;
	marsMoon1->rotSpeed = 1.5;
	marsMoon1->name = "marsMoon1";

	//marsMoon2
	QuadSphere * marsMoon2 = new QuadSphere;
	marsMoon2->colour = Colour4(1,1,1,1);
	marsMoon2->position = Vector3(0,0,0.3);
	//notice having to flip the sphere sideways to
	marsMoon2->rpy = Vector3(-90,0,0);
	marsMoon2->rpySpeed = Vector3(0,0,0.4);
	marsMoon2->scale = Vector3(0.1,0.1,0.1);
	marsMoon2->textureID = 4;
	marsMoon2->objRadius = 3;
	marsMoon2->rotSpeed = 1;
	marsMoon2->name = "marsMoon2";

	//Your Spaceship.
	mySpaceShip = new Mesh;
	mySpaceShip->loadModel("models/rocket.obj");
	mySpaceShip->textureID = 6;
	shipPos = Vector3(5,0,0);
	mySpaceShip->colour = Colour4(1,1,0,1);
	mySpaceShip->scale = Vector3(0.25,0.25,0.25);
	mySpaceShip->position = shipPos;
	mySpaceShip->objRadius = shipPos.x;
	mySpaceShip->name = "Ship";
	mySpaceShip->rotSpeed = earth->rpySpeed.z;
	mySpaceShip->rpySpeed = mars->rpySpeed;



	//add the earth to the sun.
	sun->AddChild(earth);

	//add mars to the sun
	sun->AddChild(mars);

	//add the moon to the earth
	earth->AddChild(moon);
	earth->AddChild(mySpaceShip);

	mars->AddChild(marsMoon1);
	mars->AddChild(marsMoon2);

	SceneRoot.push_back(sky);
	SceneRoot.push_back(sun);


	sun->absPos = sun->position;
	earth->absPos = sun->position + earth->position;
	mars->absPos = sun->position + mars->position;
	moon->absPos = sun->position + earth->position + moon->position;
	mySpaceShip->absPos = sun->position + earth->position + mySpaceShip->position;


//	SceneRoot.push_back(mySpaceShip);
	for (unsigned int i = 0; i < SceneRoot.size(); ++i) {
		SceneRoot[i]->setTimeScale(timeScale);
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glEnable(GL_NORMALIZE);
	glClearColor(1.0, 1.0, 1.0, 0.0);
	//setup double buffering
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	//set up the window
	glutInitWindowSize(X_RES, Y_RES);
	glutInitWindowPosition(0, 0);
	glutCreateWindow ("Voyage to Mars");

	//assign the callback functions
	glutDisplayFunc(display);
//	glutSpecialFunc(SpecialInput);
	glutKeyboardFunc(keyboard);


	LoadDefaultScene();//load the scene
	animateSceneTimer(5);//set the animation timer

	//GOOOOWWW!!
	glutMainLoop();
	return 0;
}
