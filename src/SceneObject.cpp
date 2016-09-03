/*
 * BasicSceneObject.cpp
 *
 * This is the scene object superclass, providing
 * the basics that you need to display objects on screen.
 *
 * by Matt Cabanag
 *
 */

#include "SceneObject.h"
#include <iostream>
#include <math.h>

SceneObject::SceneObject()
{
	active = true;
	displayActive = true;
	lighting = true;
	rotateOnPoint = true;
	isPlanet = false;
	launched = false;
	destReached = false;
	cullAll = false;
	favourable = false;

	child = NULL;
	flightState = ReadyToLaunch;

	position = Vector3();
	offset = Vector3();
	absPos = Vector3();
	speed = 5;
	scale = Vector3(1,1,1);
	rpy = Vector3();//yaw pitch and roll
	rpySpeed = Vector3();//yaw pitch roll speed
	destination = absPos;

	objPitch 	= 0;
	objYaw 		= 0;
	objRadius 	= 0;
	rotSpeed 	= 0;
	pitch		= 0;
	yaw 		= 0;
	timeScale	= 1;

	colour.r = 1;
	colour.g = 1;
	colour.b = 1;
	colour.a = 1;

	textureID = -1;//this means it doesn't have a texture ID.
}


//Updating the logic in the scene object is simply
//adding its delta values to its current values.
//That is, add the speed vector to its position and
//add its roll/pitch/yaw speed values to its current
//yaw/pitch/roll.
void SceneObject::Update()
{
	if(objRadius > 0 && !launched)
	{
		pitch = objPitch * (M_PI / 180);
		yaw = objYaw * (M_PI / 180);

		//PITCH
		objY = (0 * cos(pitch)) + (objRadius * -sin(pitch));
		objZ = (0 * sin(pitch)) + (objRadius * cos(pitch));

		//HEADING
		objX = (0 * cos(yaw)) + (objZ * sin(yaw));
		objZ = (0 * -sin(yaw)) + (objZ * cos(yaw));

		objYaw += rotSpeed * timeScale;

		position = Vector3(objX, objY, objZ);
	}
	else if(launched)
	{
		objRadius = 5;
		direction = destination - position;
		direction.normalize();

		omega = atan(-direction.z / direction.x);
		omega *= 180.0f/M_PI;

//		//rotate toward direction
//		cross = Vector3((position.y*destination.z) - (position.z*destination.y),
//								(position.z*destination.x) - (position.x*destination.z),
//								(position.x*destination.y) - (position.y*destination.x));
//
//		dot = (position.x * destination.x +
//					 position.y * destination.y +
//					 position.z * destination.z);

		position += direction * speed * (5.0f/1000) * timeScale;
		if(distance.distance(destination) < objRadius){
			launched = false;
			destReached = true;
			destination = absPos;
		}
	}

	rpy +=  Vector3(rpySpeed.x * timeScale, rpySpeed.y * timeScale, rpySpeed.z * timeScale);

	UpdateChildren();
}

void SceneObject::DisplayToConsole()
{
	cout<<rpy.x<<","<<rpy.y<<","<<rpy.z<<","<<endl;
}

void SceneObject::Display()
{
	//deal with the lighting
	if(!lighting)
	{
		glDisable(GL_LIGHTING);
		glColor3f(colour.r,colour.g,colour.b);
	}
	else
	{
		glEnable(GL_LIGHTING);
		GLfloat col[4] = { colour.r,colour.g,colour.b, colour.a};
		GLfloat specular[4] = { 1, 1, 1, 1};
		GLfloat shininess = 50;

//		if(name == "Sky"){
//			glMaterialfv(GL_BACK, GL_AMBIENT_AND_DIFFUSE, col);
//			glMaterialfv(GL_BACK, GL_SPECULAR, specular );
//			glMaterialf(GL_BACK, GL_SHININESS, shininess);
//		}
//		else{
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, col);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular );
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
//		}
	}

	//deal with culling
	if(name == "Sky")
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
	}
	else
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}

	if(cullAll == true){
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT_AND_BACK);
	}

	//deal with the texturing
	if(textureID >= 0)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,textureID);

	}
	else
	{
		glDisable(GL_TEXTURE_2D);
	}


	glPushMatrix();

		glTranslatef(position.x,position.y,position.z);
		glPushMatrix();

			if(name == "Ship")
			{
//				cout << omega << endl;
				omega  = sqrtf(omega*omega);
				glRotatef(omega,0,1,0);//yaw
				glRotatef(90,0,0,1);//pitch
//				glRotatef(omega,1,0,0);//roll
				if(flightState == LandedOnMars || flightState == ReadyToDescend)
				{
					glRotatef(rpy.z,0,1,0);//yaw
					glRotatef(rpy.y,0,0,1);//pitch
					glRotatef(rpy.x,1,0,0);//roll
				}
			}
			else{
				glRotatef(rpy.z,0,1,0);//yaw
				glRotatef(rpy.y,0,0,1);//pitch
				glRotatef(rpy.x,1,0,0);//roll
			}
			glScalef(scale.x,scale.y,scale.z);
			DrawModel();
		glPopMatrix();

		DrawChildren();

	glPopMatrix();
	glEnable(GL_LIGHTING);
}

void SceneObject::DrawModel()
{
	glutSolidCube(3);
	glutSolidSphere(2,20,20);
}

//TODO: This code can handle one child. How do you handle multiple children?
void SceneObject::DrawChildren()
{
	//but  display your children.
	if(children.size() > 0)
	{
		glPushMatrix();
			for (unsigned int i = 0; i < children.size(); ++i) {
				//glTranslatef(children.at(i)->offset.x,children.at(i)->offset.y,children.at(i)->offset.z);
				glPushMatrix();
					children.at(i)->Display();
				glPopMatrix();
			}
		glPopMatrix();
	}
}

//TODO: This code can handle one child. How do you handle multiple children?
void SceneObject::UpdateChildren()
{
	if(children.size() > 0){
		for (unsigned int i = 0; i < children.size(); ++i) {
			children[i]->Update();
		}
	}
}

void SceneObject::AddChild(SceneObject* newChild)
{
	child = newChild;
	children.push_back(child);
}

void SceneObject::removeChild(SceneObject* newChild)
{
	for (unsigned int i = 0; i < children.size(); ++i) {
		if(children.at(i) == newChild){
			children.erase(children.begin() + i);
		}
//		else
//			cout << "Child Not Found" << endl;
//		children.at(i)->removeChild(newChild);
	}
}

void SceneObject::rotateObj(float angle, Vector3 rot)
{
//	angle = 45;
	cout << angle << endl;
	glPushMatrix();
		glRotatef(angle,0,cross.z,0);//yaw
		glRotatef(angle,0,0,cross.y);//pitch
		glRotatef(angle,cross.x,0,0);//roll
	glPopMatrix();
}

void SceneObject::setTimeScale(float time)
{
	timeScale = time;
	for(unsigned int i = 0; i < children.size(); i++)
	{
		children[i]->setTimeScale(time);
	}
}

void SceneObject::getChildAbsPos(Vector3 abs)
{
	absPos = position + abs;
//	cout << name << ": " << absPos.x << ", " << absPos.y << ", " << absPos.z << endl;

	for (unsigned int i = 0; i < children.size(); ++i) {
		if(children[i] != NULL)
			children[i]->getChildAbsPos(absPos);
	}
}

void SceneObject::drawPath()
{
	if( objRadius > 0)
	{
		float radius = objRadius;
		const float d2r = M_PI/180;
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
		glPushMatrix();
		glBegin(GL_LINE_LOOP);
			  glColor3f(1,1,1);
				 for (int i=0; i < 360; i++)
				 {
					float degInRad = i*d2r;
					Vector3 line = Vector3(cos(degInRad)*radius, 0, sin(degInRad)*radius);
					Vector3 linePos = absPos + line;
					linePos += Vector3(-position.x, -position.y, -position.z);
					glVertex3f(linePos.x, linePos.y, linePos.z);
				 }
		glEnd();
		glPopMatrix();
		glEnable(GL_LIGHTING);

	}

	for (unsigned int i = 0; i < children.size(); ++i) {
				children[i]->drawPath();
	}
}

vector<SceneObject*> SceneObject::getChildren()
{
	return children;
}

// returns the angle mod 360 in degrees
float SceneObject::getAngle()
{
	return fmod((objYaw * (180 / M_PI)),360);
}
