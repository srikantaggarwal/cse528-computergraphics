//
//  Vector.cpp
//  CGProject
//
//  Created by Srikant Aggarwal on 31/10/14.
//  Copyright (c) 2014 Srikant Aggarwal. All rights reserved.
//

#include "Vector.h"

/***************************************************
 * Constructor
 **************************************************/
Vector::Vector(float x_value, float y_value, float z_value)
    :x(x_value), y(y_value), z(z_value)
{
}

/***************************************************
 * operator/ overloading
 **************************************************/
Vector Vector::operator/(float f) const
{
    return Vector(x/f, y/f, z/f);
}

/***************************************************
 * operator* overloading
 **************************************************/
Vector Vector::operator*(float f) const
{
    return Vector(x*f, y*f, z*f);
}

/***************************************************
 * operator+ overloading
 **************************************************/
Vector Vector::operator+(Vector v) const
{
    return Vector (x+v.x, y+v.y, z+v.z);
}

/***************************************************
 * operator- overloading
 **************************************************/
Vector Vector::operator-(Vector v) const
{
    return Vector (x-v.x, y-v.y, z-v.z);
}

/***************************************************
 * Method returns magnitude of vector
 **************************************************/
float Vector::magnitude()
{
    return sqrt(x*x + y*y + z*z);
}

/***************************************************
 * method returns unit vector in direction of the 
 * vector
 **************************************************/
Vector Vector::normalise()
{
    float mag = sqrt(x*x + y*y + z*z);
    return Vector(x/mag, y/mag, z/mag);
}

/***************************************************
 * Method projects another vector on this vector
 **************************************************/
Vector Vector::project(Vector v)
{
    Vector normVt = normalise();
    float mag = normVt.x*v.x + normVt.y*v.y + normVt.z*v.z;
    return Vector(normVt.x*mag, normVt.y*mag, normVt.z*mag);
}

/***************************************************
 * Destructor
 **************************************************/
Vector::~Vector(void) {}
