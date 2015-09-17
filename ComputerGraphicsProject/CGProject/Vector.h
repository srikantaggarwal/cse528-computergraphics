//
//  Vector.h
//  CGProject
//
//  Created by Srikant Aggarwal on 31/10/14.
//  Copyright (c) 2014 Srikant Aggarwal. All rights reserved.
//

#ifndef __CGProject__Vector__
#define __CGProject__Vector__

#include <iostream>
#include <cmath>

class Vector {
public:
    Vector(float x = 0.0f, float y = 0.0f, float z = 0.0f);
   
    ~Vector(void);
    
    Vector operator+(Vector v) const;
    Vector operator-(Vector v) const;
    
    Vector operator/(float f) const;
    Vector operator*(float f) const;
    
    float magnitude();
    Vector normalise();
    Vector project(Vector v);
    
    float x, y, z;
};

#endif /* defined(__CGProject__Vector__) */
