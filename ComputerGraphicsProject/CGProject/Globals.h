//
//  Globals.h
//  CGProject
//
//  Created by Srikant Aggarwal on 02/11/14.
//  Copyright (c) 2014 Srikant Aggarwal. All rights reserved.
//

#ifndef CGProject_Globals_h
#define CGProject_Globals_h

enum DEFORMATION_TYPE
{
    RIGID,
    RIGID_ROTATED,
    LINEAR,
    LINEAR_ROTATED,
    QUADRATIC
};

typedef struct Rectangle Rect;

extern int mode;
extern float alpha;
extern float beta;
extern int cube_color;
extern float groundLevel;
extern float elasticity;

#endif
