//
//  Particle.h
//  CGProject
//
//  Created by Srikant Aggarwal on 31/10/14.
//  Copyright (c) 2014 Srikant Aggarwal. All rights reserved.
//

#ifndef __CGProject__Particle__
#define __CGProject__Particle__

#include <iostream>

#include "Globals.h"
#include "Vector.h"

#include <vector>

using namespace std;

class Particle
{
public:
    Particle(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 1.0f, float m = 1.0f, float r = 0.001f, Vector f = Vector(0.0f, -0.0000098f, 0.0f));
    Particle(Vector p, float m, float r = 0.001f, Vector f = Vector(0.0f, -0.0000098f, 0.0f));
    void initialize(float y);
    void update(long h, vector<Particle> particles, int exclude);
    ~Particle(void);
    Vector getOriginalPosition(void);
    Vector getTempPosition(void);
    Vector getPosition(void);
    void setGoalPosition(Vector gp);
    void setTempPosition(Vector tp);
    Vector getVelocity(void);
    void setVelocity(Vector v);
    Vector getAcceleration(void);
    Vector getForce(void);
    float getMass(void);
    bool canMove(Vector p, vector<Particle> particles, int exclude);
    float getRadius();
    
private:
    Vector original_position;
    Vector goal_position;
    Vector temp_position;
    Vector position;
    Vector velocity;
    Vector acceleration;
    Vector force;
    float mass;
    float radius;
};

#endif /* defined(__CGProject__Particle__) */
