/** Functions for drawing shapes like a torus and a cube.
    Replaces ones from glut that don't generate texture coordinates
    Written by Tim Lambert based on code in glut
 */


/* Some of this code was copied from glut and hence we have to include
   the notice below */

/* Copyright (c) Mark J. Kilgard, 1994, 1997. */

/**
(c) Copyright 1993, Silicon Graphics, Inc.

ALL RIGHTS RESERVED

Permission to use, copy, modify, and distribute this software
for any purpose and without fee is hereby granted, provided
that the above copyright notice appear in all copies and that
both the copyright notice and this permission notice appear in
supporting documentation, and that the name of Silicon
Graphics, Inc. not be used in advertising or publicity
pertaining to distribution of the software without specific,
written prior permission.

THE MATERIAL EMBODIED ON THIS SOFTWARE IS PROVIDED TO YOU
"AS-IS" AND WITHOUT WARRANTY OF ANY KIND, EXPRESS, IMPLIED OR
OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.  IN NO
EVENT SHALL SILICON GRAPHICS, INC.  BE LIABLE TO YOU OR ANYONE
ELSE FOR ANY DIRECT, SPECIAL, INCIDENTAL, INDIRECT OR
CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER,
INCLUDING WITHOUT LIMITATION, LOSS OF PROFIT, LOSS OF USE,
SAVINGS OR REVENUE, OR THE CLAIMS OF THIRD PARTIES, WHETHER OR
NOT SILICON GRAPHICS, INC.  HAS BEEN ADVISED OF THE POSSIBILITY
OF SUCH LOSS, HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
ARISING OUT OF OR IN CONNECTION WITH THE POSSESSION, USE OR
PERFORMANCE OF THIS SOFTWARE.

US Government Users Restricted Rights

Use, duplication, or disclosure by the Government is subject to
restrictions set forth in FAR 52.227.19(c)(2) or subparagraph
(c)(1)(ii) of the Rights in Technical Data and Computer
Software clause at DFARS 252.227-7013 and/or in similar or
successor clauses in the FAR or the DOD or NASA FAR
Supplement.  Unpublished-- rights reserved under the copyright
laws of the United States.  Contractor/manufacturer is Silicon
Graphics, Inc., 2011 N.  Shoreline Blvd., Mountain View, CA
94039-7311.

OpenGL(TM) is a trademark of Silicon Graphics, Inc.
 */
#include <math.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <gl\glut.h>
#include "Shapes.h"

void torus(double r, double R, int nsides, int rings){
	int i, j;
	double theta, phi, theta1;
	double cosTheta, sinTheta;
	double cosTheta1, sinTheta1;
	double ringDelta, sideDelta;
	double s,s1,t,sDelta,tDelta;

	ringDelta = 2.0 * M_PI / rings;
	sideDelta = 2.0 * M_PI / nsides;
	sDelta = 1.0 / rings;
	tDelta = 1.0 / nsides;

	theta = 0.0;
	cosTheta = 1.0;
	sinTheta = 0.0;
	s = 0.0;
	for (i = rings - 1; i >= 0; i--) {
		theta1 = theta + ringDelta;
		s1 = s + sDelta;
		cosTheta1 = cos(theta1);
		sinTheta1 = sin(theta1);
		glBegin(GL_QUAD_STRIP);
		phi = 0.0;
		t = 0.0;
		for (j = nsides; j >= 0; j--) {
			double cosPhi, sinPhi, dist;

			phi += sideDelta;
			t += tDelta;
			cosPhi = cos(phi);
			sinPhi = sin(phi);
			dist = R + r * cosPhi;

			glNormal3d(cosTheta1 * cosPhi, -sinTheta1 * cosPhi, sinPhi);
			glTexCoord2d(s1,t);
			glVertex3d(cosTheta1 * dist, -sinTheta1 * dist, r * sinPhi);
			glNormal3d(cosTheta * cosPhi, -sinTheta * cosPhi, sinPhi);
			glTexCoord2d(s,t);
			glVertex3d(cosTheta * dist, -sinTheta * dist,  r * sinPhi);
		}
		glEnd();
		theta = theta1;
		s = s1;
		cosTheta = cosTheta1;
		sinTheta = sinTheta1;
	}
}


static float n[6][3] =
{
		{-1.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f},
		{1.0f, 0.0f, 0.0f},
		{0.0f, -1.0f, 0.0f},
		{0.0f, 0.0f, 1.0f},
		{0.0f, 0.0f, -1.0f}
};
static int faces[6][4] =
{
		{0, 1, 2, 3},
		{3, 2, 6, 7},
		{7, 6, 5, 4},
		{4, 5, 1, 0},
		{5, 6, 2, 1},
		{7, 4, 0, 3}
};

void cube(double size){
	float v[8][3];
	int i;

	v[0][0] = v[1][0] = v[2][0] = v[3][0] = (float)-size / 2;
	v[4][0] = v[5][0] = v[6][0] = v[7][0] = (float)size / 2;
	v[0][1] = v[1][1] = v[4][1] = v[5][1] = (float)-size / 2;
	v[2][1] = v[3][1] = v[6][1] = v[7][1] = (float)size / 2;
	v[0][2] = v[3][2] = v[4][2] = v[7][2] = (float)-size / 2;
	v[1][2] = v[2][2] = v[5][2] = v[6][2] = (float)size / 2;

	for (i = 5; i >= 0; i--) {
		glBegin(GL_POLYGON);
		glNormal3fv(n[i]);
		glTexCoord2f(0,0);
		glVertex3fv(v[faces[i][0]]);
		glTexCoord2f(2,0);
		glVertex3fv(v[faces[i][1]]);
		glTexCoord2f(2,2);
		glVertex3fv(v[faces[i][2]]);
		glTexCoord2f(0,2);
		glVertex3fv(v[faces[i][3]]);
		glEnd();
	}
}

//modified by Matt to show rotated UV mapping from cube()
void angledTexCube(double size){
	float v[8][3];
	int i;

	v[0][0] = v[1][0] = v[2][0] = v[3][0] = (float)-size / 2;
	v[4][0] = v[5][0] = v[6][0] = v[7][0] = (float)size / 2;
	v[0][1] = v[1][1] = v[4][1] = v[5][1] = (float)-size / 2;
	v[2][1] = v[3][1] = v[6][1] = v[7][1] = (float)size / 2;
	v[0][2] = v[3][2] = v[4][2] = v[7][2] = (float)-size / 2;
	v[1][2] = v[2][2] = v[5][2] = v[6][2] = (float)size / 2;

	for (i = 5; i >= 0; i--) {
		glBegin(GL_POLYGON);
		glNormal3fv(n[i]);
		glTexCoord2f(0.5,0);
		glVertex3fv(v[faces[i][0]]);
		glTexCoord2f(1,0.5);
		glVertex3fv(v[faces[i][1]]);
		glTexCoord2f(0.5,1);
		glVertex3fv(v[faces[i][2]]);
		glTexCoord2f(0,0.5);
		glVertex3fv(v[faces[i][3]]);
		glEnd();
	}
}

//default profile -- a wine glass
static double profile[][2] = {{0.26,0},{0.26,0},{0.09,0.02},{0.07,0.02},{0.04,0.05},{0.02,0.05},{0.02,0.38},{0.05,0.46},{0.14,0.56},{0.41,0.98},{0.4,0.99},{0.11,0.57},{0.04,0.51},{0,0.5}};

//surface of revolution given array containing points of the profile
// profile[0][0] is x coord of first point,
// profile[0][1] is y coord of first point,
void revolution(int level, double profile[][2], int npoints){

	double h = 1.0f/level;
	for (int i = 1; i < npoints; i++){
		double s = i/(double)npoints;
		glBegin(GL_TRIANGLE_STRIP);
		for (int j = 0; j <= level; j++){
			double t = j*h;
			double angle = t*2.0*M_PI;
			double cs = cos(angle);
			double sn = sin(angle);
			glNormal3d(cs,0.0,sn); //normal to a circle is radius

			glTexCoord2d(s-h,t);
			glVertex3d(profile[i-1][0]*cs,profile[i-1][1],profile[i-1][0]*sn);
			glTexCoord2d(s,t);
			glVertex3d(profile[i][0]*cs,profile[i][1],profile[i][0]*sn);
		}
		glEnd();
	}
}

void revolution(int level){
revolution(level,profile,14 );
}
