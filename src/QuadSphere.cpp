/*
 * QuadSphere.cpp
 *
 *  Created on: 29/04/2014
 *      Author: AVataRR
 */

#include "QuadSphere.h"
#include <math.h>

void cg_SolidSphere( GLint slices, GLint stacks );
void gluSphere();

void QuadSphere::DrawModel()
{
	gluSphere();
	//DrawChildren();
}

//example lifted from:
//http://stackoverflow.com/questions/7701377/opengl-lighting-and-shading-on-a-gl-quad-strip
//modified by Matt Cabanag
void cg_SolidSphere( GLint slices, GLint stacks )
{
    float x, y, z ;
    glBegin( GL_QUAD_STRIP ) ;
    for( float lat = 0 ; lat < M_PI ; lat = lat + M_PI/stacks )
    {

        for( float lon = 0 ; lon < 2*M_PI ; lon = lon + 2*M_PI/slices )
        {
        	x = cosf( lat ) * sinf( lon ) ;
        	y = sinf( lat ) * sinf( lon ) ;
        	z = cosf( lon ) ;
        	glNormal3f( x, y, z ) ;
        	glVertex3f( x, y, z ) ;
        	x = cosf( lat + M_PI/stacks ) * sinf( lon ) ;
        	y = sinf( lat + M_PI/stacks ) * sinf( lon ) ;
        	z = cosf( lon ) ;
        	glNormal3f( x, y, z ) ;
        	glVertex3f( x, y, z ) ;
        }

    }
    glEnd() ;
}

void gluSphere()
{
	GLUquadric* qobj = gluNewQuadric();
	gluQuadricTexture(qobj, true);
	gluSphere(qobj,2,100,100);
}


