/*
 * UtilityStructs.h
 *
 * Some useful structs that we can use.
 *
 * COMP330 Assignment 3 - 2015
 *
 * Template by Matt Cabanag
 */

#include <vector>
#include <string>
#include <iostream>
#include <math.h>
using namespace std;

//Nothing special, just a struct to store three numbers in a
//single struct, with some operator overloads for addition.
#ifndef VECTOR2_
#define VECTOR2_
struct Vector2
{
	float x, y;

	Vector2()
	{
		x = 0;
		y = 0;
	}

	Vector2(float nx, float ny)
	{
		x = nx;
		y = ny;
	}

	void operator = (const Vector2 &other)
	{
		x = other.x;
		y = other.y;
	}

	Vector2 operator +(const Vector2 &other) const
	{
		return Vector2(other.x + x, other.y + y);
	}

	void operator += (const Vector2 &other)
	{
		x += other.x;
		y += other.y;
	}

};
#endif

//Nothing special, just a struct to store three numbers in a
//single struct, with some operator overloads for addition.
#ifndef VECTOR3_
#define VECTOR3_
struct Vector3
{
	float x, y, z;

	Vector3()
	{
		x = 0;
		y = 0;
		z = 0;
	}

	Vector3(float nx, float ny, float nz)
	{
		x = nx;
		y = ny;
		z = nz;
	}

	void operator = (const Vector3 &other)
	{
		x = other.x;
		y = other.y;
		z = other.z;
	}

	Vector3 operator +(const Vector3 &other) const
	{
		return Vector3(other.x + x, other.y + y, other.z + z);
	}

	void operator += (const Vector3 &other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
	}

	Vector3 operator - (const Vector3 &other)
			{
				x -= other.x;
				y -= other.y;
				z -= other.z;
				return Vector3(x,y,z);
			}

	void operator += (const float &other)
		{
			x += other;
			y += other;
			z += other;
		}

	bool operator != (const Vector3 &other)
		{
			if(x == other.x || y == other.y || z == other.z)
				return false;
			return true;
		}

	bool operator == (const Vector3 &other)
		{
			if(x != other.x || y != other.y || z != other.z)
				return false;
			return true;
		}

	Vector3 operator * (const float &other)
		{
			x *= other;
			y *= other;
			z *= other;
			return Vector3(x,y,z);
		}

	void operator *= (const float &other)
		{
			x *= other;
			y *= other;
			z *= other;
		}

	void toString()
	{
		std::cout << "x: " << x << " y: " << y << " z: " << z << std::endl;
	}

	void normalize()
	{
		float len = sqrtf(x*x + y*y + z*z);
		x = x/len;
		y = y/len;
		z = z/len;
	}

	float distance(Vector3 end)
	{
		float len = sqrtf((x - end.x)*(x - end.x) +
						  (y - end.y)*(y - end.y) +
						  (z - end.z)*(z - end.z));
		return len;
	}

};
#endif

//Storage struct for red, green, blue, alpha
#ifndef COLOUR4_
#define COLOUR4_
struct Colour4
{
	float r,g,b,a;

	Colour4()
	{
		r = 1;
		g = 1;
		b = 1;
		a = 1;
	}

	Colour4(float nr, float ng, float nb, float na)
	{
		r = nr;
		g = ng;
		b = nb;
		a = na;
	}

	void operator = (const Colour4 &other)
	{
		r = other.r;
		g = other.g;
		b = other.b;
		a = other.a;
	}

	//returns a colour4 value based on a given string.
	static Colour4 GetColour(string s)
	{
		if(s == "white")
			return Colour4(1,1,1,1);

		if(s == "red")
			return Colour4(1,0,0,1);

		if(s == "green")
			return Colour4(0,1,0,1);

		if(s == "blue")
			return Colour4(0,0,1,0);

		return Colour4(0,0,0,0);
	}
};
#endif

#ifndef FACE_
#define FACE_
struct Face {
    std::vector<Vector3> vd; // vertex data

    void push_back(Vector3 vec) {
        vd.push_back(vec);
    }

    Vector3 at(int index) {
        return vd.at(index);
    }
};
#endif

//Storage struct for raw scene data.
#ifndef RAWSCENEDATA_
#define RAWSCENEDATA_
struct RawSceneData
{
	string type, colour;
	int cPairs;
	float initRotation;//initial rotation
	vector<Vector2> pointList;
};
#endif
