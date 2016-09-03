/*
 * Model.h
 *
 *  Created on: May 23, 2015
 *      Author: Brett
 */

/*
The following code has been extraced from Obj Loader
by HomegamesKiro©. Copyright (C) <2013>  <HomegamesKiro>

It has been modified and formatted for Mars Voyage
by Brett Sargeant, 23/5/2015.
*/

#ifndef MODEL_H_
#define MODEL_H_

#include <vector>
#include <iostream>
#include <fstream>
#include "UtilityStructs.h"
#include <GL/glut.h>
#include "SOIL.h"
#include "SceneObject.h"


using namespace std;

class Model : public SceneObject{
public:
	void addModel(string name, string filename);
	void addTexture(string name, string filename);
	int Count(string str,char tofind);
	unsigned int GetTexture(string name);
	bool CheckTexture(string name);

private:

	virtual void DrawModel();

	struct OBJFACE
	{
		vector<int> f;
		vector<int> t;
		vector<int> n;
	};

	 struct wavefrontobject
	 {  vector<string> oname;
	    vector<string> mtlname;
		vector<bool> UseTexture;
	    vector<bool> UseNormals;
		vector<Vector3> Vertex;
		vector<Vector3> Normals;
		vector<Vector2> Texture;
		vector<OBJFACE>  Face;

	 };

	struct ResTexture
	{
		string name;
		unsigned int container;

	};
	struct ResModel
	{
		string name;
		vector<unsigned int> container;
		wavefrontobject mod;
	};

	vector<ResTexture> Textures;
	vector<ResModel>   Models;

};

#endif /* MODEL_H_ */
