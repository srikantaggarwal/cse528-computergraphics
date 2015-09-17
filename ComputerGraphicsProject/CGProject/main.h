//
//  main.h
//  CGProject
//
//  Created by Srikant Aggarwal on 29/10/14.
//  Copyright (c) 2014 Srikant Aggarwal. All rights reserved.
//

#ifndef CGProject_main_h
#define CGProject_main_h

#include <iostream>
using namespace std;

#include <GLUT/GLUT.h>

#include "Globals.h"
#include "DeformableObject.h"
#include "Bitmap.h"

#include <ctime>

//Mode choosen (RIGID, LINEAR, QUADRATIC)
int mode;

float alpha;
float beta;
int cube_color;

//Level of the ground
float groundLevel;

//Elasticity offered by the system
float elasticity;

#endif
