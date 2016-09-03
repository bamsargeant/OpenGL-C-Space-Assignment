/*
 * QuadCube.cpp
 *
 *  Created on: 29/04/2014
 *      Author: AVataRR
 */

#include "QuadCube.h"

void drawNoNormals();
void drawQuadCubeWithNormals();
void drawQuadCubeWithTextCoords();

void QuadCube:: DrawModel()
{
	drawQuadCubeWithTextCoords();
}

void drawQuadCubeNoNormals()
{
	 glBegin(GL_QUADS);

		glVertex3f( 0.5f, 0.5f, -0.5f);			// Top Right (Top)
		glVertex3f( -0.5f, 0.5f, -0.5f);		// Top Left (Top)
		glVertex3f( -0.5f, 0.5f, 0.5f);			// Bottom Left (Top)
		glVertex3f( 0.5f, 0.5f, 0.5f);			// Bottom Right (Top)

		glVertex3f( -0.5f, -0.5f, -0.5f);		// Top Left (Bottom)
		glVertex3f( 0.5f, -0.5f, -0.5f);		// Top Right (Bottom)
		glVertex3f( 0.5f, -0.5f, 0.5f);			// Bottom Right (Bottom)
		glVertex3f( -0.5f, -0.5f, 0.5f);		// Bottom Left (Bottom)

		glVertex3f( 0.5f, 0.5f, 0.5f);			// Top Right Of The Quad (Front)
		glVertex3f(-0.5f, 0.5f, 0.5f);			// Top Left Of The Quad (Front)
		glVertex3f(-0.5f,-0.5f, 0.5f);			// Bottom Left Of The Quad (Front)
		glVertex3f( 0.5f,-0.5f, 0.5f);			// Bottom Right Of The Quad (Front)

		glVertex3f( 0.5f,-0.5f,-0.5f);			// Bottom Left Of The Quad (Back)
		glVertex3f(-0.5f,-0.5f,-0.5f);			// Bottom Right Of The Quad (Back)
		glVertex3f(-0.5f, 0.5f,-0.5f);			// Top Right Of The Quad (Back)
		glVertex3f( 0.5f, 0.5f,-0.5f);			// Top Left Of The Quad (Back)

		glVertex3f(-0.5f, 0.5f, 0.5f);			// Top Right Of The Quad (Left)
		glVertex3f(-0.5f, 0.5f,-0.5f);			// Top Left Of The Quad (Left)
		glVertex3f(-0.5f,-0.5f,-0.5f);			// Bottom Left Of The Quad (Left)
		glVertex3f(-0.5f,-0.5f, 0.5f);			// Bottom Right Of The Quad (Left)

		glVertex3f( 0.5f, 0.5f,-0.5f);			// Top Right Of The Quad (Right)
		glVertex3f( 0.5f, 0.5f, 0.5f);			// Top Left Of The Quad (Right)
		glVertex3f( 0.5f,-0.5f, 0.5f);			// Bottom Left Of The Quad (Right)
		glVertex3f( 0.5f,-0.5f,-0.5f);			// Bottom Right Of The Quad (Right)

	    glEnd();
}

void drawQuadCubeWithTextCoords()
{
	glBegin(GL_QUADS);

			//yep, we got texture coordinates here.
			glNormal3f(0,1,0);
			glTexCoord2f(1,1);
			glVertex3f( 0.5f, 0.5f, -0.5f);			// Top Right (Top)
			glTexCoord2f(0,1);
			glVertex3f( -0.5f, 0.5f, -0.5f);		// Top Left (Top)
			glTexCoord2f(0,0);
			glVertex3f( -0.5f, 0.5f, 0.5f);			// Bottom Left (Top)
			glTexCoord2f(1,0);
			glVertex3f( 0.5f, 0.5f, 0.5f);			// Bottom Right (Top)

			//todo:what are the texture coordinates for this face?
			glNormal3f(0,-1,0);
			glVertex3f( -0.5f, -0.5f, -0.5f);		// Top Left (Bottom)
			glVertex3f( 0.5f, -0.5f, -0.5f);		// Top Right (Bottom)
			glVertex3f( 0.5f, -0.5f, 0.5f);			// Bottom Right (Bottom)
			glVertex3f( -0.5f, -0.5f, 0.5f);		// Bottom Left (Bottom)

			//todo:what are the texture coordinates for this face?
			glNormal3f(0,0,1);
			glVertex3f( 0.5f, 0.5f, 0.5f);			// Top Right Of The Quad (Front)
			glVertex3f(-0.5f, 0.5f, 0.5f);			// Top Left Of The Quad (Front)
			glVertex3f(-0.5f,-0.5f, 0.5f);			// Bottom Left Of The Quad (Front)
			glVertex3f( 0.5f,-0.5f, 0.5f);			// Bottom Right Of The Quad (Front)

			//todo:what are the texture coordinates for this face?
			glNormal3f(0,0,-1);
			glVertex3f( 0.5f,-0.5f,-0.5f);			// Bottom Left Of The Quad (Back)
			glVertex3f(-0.5f,-0.5f,-0.5f);			// Bottom Right Of The Quad (Back)
			glVertex3f(-0.5f, 0.5f,-0.5f);			// Top Right Of The Quad (Back)
			glVertex3f( 0.5f, 0.5f,-0.5f);			// Top Left Of The Quad (Back)

			//todo:what are the texture coordinates for this face?
			glNormal3f(-1,0,0);
			glVertex3f(-0.5f, 0.5f, 0.5f);			// Top Right Of The Quad (Left)
			glVertex3f(-0.5f, 0.5f,-0.5f);			// Top Left Of The Quad (Left)
			glVertex3f(-0.5f,-0.5f,-0.5f);			// Bottom Left Of The Quad (Left)
			glVertex3f(-0.5f,-0.5f, 0.5f);			// Bottom Right Of The Quad (Left)

			//todo:what are the texture coordinates for this face?
			glNormal3f(1,0,0);
			glVertex3f( 0.5f, 0.5f,-0.5f);			// Top Right Of The Quad (Right)
			glVertex3f( 0.5f, 0.5f, 0.5f);			// Top Left Of The Quad (Right)
			glVertex3f( 0.5f,-0.5f, 0.5f);			// Bottom Left Of The Quad (Right)
			glVertex3f( 0.5f,-0.5f,-0.5f);			// Bottom Right Of The Quad (Right)

		glEnd();
}

void drawQuadCubeWithNormals()
{
	glBegin(GL_QUADS);

			//todo: what's the normal here?
			glNormal3f(0,1,0);
			glVertex3f( 0.5f, 0.5f, -0.5f);			// Top Right (Top)
			glVertex3f( -0.5f, 0.5f, -0.5f);		// Top Left (Top)
			glVertex3f( -0.5f, 0.5f, 0.5f);			// Bottom Left (Top)
			glVertex3f( 0.5f, 0.5f, 0.5f);			// Bottom Right (Top)

			//todo: what's the normal here?
			glNormal3f(0,-1,0);
			glVertex3f( -0.5f, -0.5f, -0.5f);		// Top Left (Bottom)
			glVertex3f( 0.5f, -0.5f, -0.5f);		// Top Right (Bottom)
			glVertex3f( 0.5f, -0.5f, 0.5f);			// Bottom Right (Bottom)
			glVertex3f( -0.5f, -0.5f, 0.5f);		// Bottom Left (Bottom)

			//todo: what's the normal here?
			glNormal3f(0,0,1);
			glVertex3f( 0.5f, 0.5f, 0.5f);			// Top Right Of The Quad (Front)
			glVertex3f(-0.5f, 0.5f, 0.5f);			// Top Left Of The Quad (Front)
			glVertex3f(-0.5f,-0.5f, 0.5f);			// Bottom Left Of The Quad (Front)
			glVertex3f( 0.5f,-0.5f, 0.5f);			// Bottom Right Of The Quad (Front)

			//todo: what's the normal here?
			glNormal3f(0,0,-1);
			glVertex3f( 0.5f,-0.5f,-0.5f);			// Bottom Left Of The Quad (Back)
			glVertex3f(-0.5f,-0.5f,-0.5f);			// Bottom Right Of The Quad (Back)
			glVertex3f(-0.5f, 0.5f,-0.5f);			// Top Right Of The Quad (Back)
			glVertex3f( 0.5f, 0.5f,-0.5f);			// Top Left Of The Quad (Back)

			//todo: what's the normal here?
			glNormal3f(-1,0,0);
			glVertex3f(-0.5f, 0.5f, 0.5f);			// Top Right Of The Quad (Left)
			glVertex3f(-0.5f, 0.5f,-0.5f);			// Top Left Of The Quad (Left)
			glVertex3f(-0.5f,-0.5f,-0.5f);			// Bottom Left Of The Quad (Left)
			glVertex3f(-0.5f,-0.5f, 0.5f);			// Bottom Right Of The Quad (Left)

			//todo: what's the normal here?
			glNormal3f(1,0,0);
			glVertex3f( 0.5f, 0.5f,-0.5f);			// Top Right Of The Quad (Right)
			glVertex3f( 0.5f, 0.5f, 0.5f);			// Top Left Of The Quad (Right)
			glVertex3f( 0.5f,-0.5f, 0.5f);			// Bottom Left Of The Quad (Right)
			glVertex3f( 0.5f,-0.5f,-0.5f);			// Bottom Right Of The Quad (Right)

		glEnd();
}


