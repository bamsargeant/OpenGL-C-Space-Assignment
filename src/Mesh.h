#ifndef MESH_H
#define MESH_H

#include "SceneObject.h"
#include "UtilityStructs.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <math.h>


/**
    Custom 3D mesh class. Capable of loading basic models from .obj files

    @author     Tom Skowron
    @version    1.0 30/05/2015
*/
class Mesh : public SceneObject
{
    public:
        Mesh();
        virtual ~Mesh(){}

    public:
        bool loadModel(std::string filename);
        virtual void DrawModel();

        template<typename T>
        string toString( const T& value );

        vector<string> splitString(string path, char delimeter);



        int numVertices();
        int numFaces();

    protected:
        vector<Vector3> v;
        vector<Vector3> n;
        vector<Vector2> t;
        vector<Face>    f;


};

#endif // MESH_H
