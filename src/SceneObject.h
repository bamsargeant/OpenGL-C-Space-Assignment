/*
 * Basic SceneObject.h
 *
 * This is the scene object superclass, providing
 * the basics that you need to display objects on screen.
 *
 * by Matt Cabanag
 */

#include "Shapes.h"
#include "UtilityStructs.h"
#include <iostream>
#include <GL/glut.h>
#include <vector>//This is essentially a C++ ArrayList. It isn't a Vector in
				//a strictly mathematical sense, but can be used as one.
				//In our case, we're using it as an ArrayList.
using namespace std;


#ifndef SCENEOBJECT_H_
#define SCENEOBJECT_H_

class SceneObject
{
	public:
		//default constructor
		SceneObject();

		SceneObject(Vector3 pos, float speed, Vector3 scale,
				Vector3 ypr, Vector3 yprSpeed, Vector3 rgb);

		enum FLIGHTSTATE{
			ReadyToLaunch = 0,
			FlyingToMars = 1,
			ReadyToDescend = 2,
			LandedOnMars = 3,
		};

		int flightState;

		//the function to update the object's attributes.
		virtual void Update();

		//display the damn thing
		virtual void Display();

		//display stuff to console
		virtual void DisplayToConsole();

		virtual void AddChild(SceneObject*);
		virtual void removeChild(SceneObject*);

		virtual void rotateObj(float angle, Vector3 rot);

		virtual void setTimeScale(float time);

		virtual void getChildAbsPos(Vector3 abs);

		virtual void drawPath();

		virtual float getAngle();
		bool favourable;
		bool cullAll;
		virtual vector<SceneObject*> getChildren();

		float objPitch;
		float objYaw;
		float objRadius;

		float pitch;
		float yaw;

		float objX;
		float objY;
		float objZ;
		float rotSpeed;
		float desiredRotSpeed;
		float flySpeed;
		float speed;
		float timeScale;

		float a;
		float b;
		float omega;
		float dot;

		Vector3 destination;
		Vector3 position;
		Vector3 offset;
		Vector3 scale;
		Vector3 rpy;//yaw pitch and roll
		Vector3 rpySpeed;//yaw pitch roll speed
		Colour4 colour;//red green blue.
		Vector3 absPos;
		Vector3 direction;
		Vector3 distance;
		Vector3 cross;


		bool lighting;
		bool displayActive;
		bool active;
		bool rotateOnPoint;
		bool isPlanet;
		bool launched;
		bool destReached;
		int textureID;
		string name;



	protected:

		vector<SceneObject*>children;
		SceneObject * child;
		virtual void DrawModel();
		virtual void DrawChildren();
		virtual void UpdateChildren();

};


#endif /* SCENEOBJECT_H_ */
