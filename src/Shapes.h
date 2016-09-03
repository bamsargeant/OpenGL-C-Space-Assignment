/*
 * Shapes.h
 *
 *  Created on: 11/04/2013
 *      Author: Tim Lambert
 */

#ifndef SHAPES_H_
#define SHAPES_H_

void torus(double r, double R, int nsides, int rings);
void cube(double size);
void revolution(int level, double profile[][2], int npoints);
void revolution(int level);

//modified by Matt to show rotated UV mapping
void angledTexCube(double size);
#endif /* SHAPES_H_ */
