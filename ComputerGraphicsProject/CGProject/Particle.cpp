//
//  Particle.cpp
//  CGProject
//
//  Created by Srikant Aggarwal on 31/10/14.
//  Copyright (c) 2014 Srikant Aggarwal. All rights reserved.
//

#include "Particle.h"

/***************************************************
 * Particle constuctor
 **************************************************/
Particle::Particle(float x, float y, float z, float w, float m, float r, Vector f) :
    original_position(x, y, z), mass(m), force(f), radius(r)
{
    velocity = Vector(0, 0, 0);
}

/***************************************************
 * Particle constructor
 **************************************************/
Particle::Particle(Vector p, float m, float r, Vector f)
:position(p.x, p.y, p.z), mass(m), force(f), radius(r)
{
    velocity = Vector(0, 0, 0);
}

/***************************************************
 * Method returns original position of particle
 **************************************************/
Vector Particle::getOriginalPosition(void)
{
    return original_position;
}

/***************************************************
 * Method initializes the particle's original and
 * current position
 **************************************************/
void Particle::initialize(float y)
{
    original_position = Vector(original_position.x, original_position.y + y, original_position.z);
    position = Vector(original_position.x, original_position.y, original_position.z);
}

/***************************************************
 * Method updates particle current velocity, position
 * Updates velocity based on whether it collides or not
 **************************************************/
void Particle::update(long h, vector<Particle> particles, int exclude)
{
    //Updating velocity and position
    Vector temp = ((goal_position - temp_position)*alpha)/h;
    velocity = velocity + temp + force*h/mass;
    position = position+velocity*h;;
    
    //Checking whether the particle collides with ground
    if (position.y < groundLevel)
    {
        //Ground bounces back the particle with a velocity in y equal to initial * elasticity factor
        velocity = Vector(velocity.x, -1*velocity.y*elasticity, velocity.z);
    }
    else
    {
        //Checks if the particle collides with some other particle
        Particle colliding_particle;
        bool is_colliding = false;
        int particle_count = particles.size();
        for (int j = 0; j < particle_count; ++j)
        {
            if (j != exclude)
            {
                Particle temp_particle = particles[j];
                
                Vector diffVt = position - temp_particle.getPosition();
                
                float mag = diffVt.magnitude();
                float min_dist = radius+temp_particle.getRadius();
                
                if (diffVt.magnitude() <= min_dist)
                {
                    colliding_particle = temp_particle;
                    is_colliding = true;
                    break;
                }
            }
        }
        
        //Updating particle's velocity if it collides with some other particle
        if (is_colliding)
        {
            Vector diffVt = position - colliding_particle.getPosition();
            Vector normalVel1 = diffVt.project(velocity);
            Vector remainingVel = velocity-normalVel1;
            velocity = (remainingVel+normalVel1)*elasticity;
            colliding_particle.setVelocity(velocity*-1);
        }
    }
}

/***************************************************
 * Setting the cureent velocity for the particle
 **************************************************/
void Particle::setVelocity(Vector v)
{
    velocity = v;
}

/***************************************************
 * Returns current position for particle
 **************************************************/
Vector Particle::getPosition(void)
{
    return position;
}

/***************************************************
 * Returns the intermediate position for particle
 **************************************************/
Vector Particle::getTempPosition(void)
{
    return temp_position;
}

/***************************************************
 * Method sets goal position for particle
 **************************************************/
void Particle::setGoalPosition(Vector gp)
{
    goal_position = gp;
}

/***************************************************
 * Method sets interim (temp) position for particle
 **************************************************/
void Particle::setTempPosition(Vector tp)
{
    temp_position = tp;
}

/***************************************************
 * Method gets current velocity for the particle
 **************************************************/
Vector Particle::getVelocity(void)
{
    return velocity;
}

/***************************************************
 * Method gets the force on the particle
 **************************************************/
Vector Particle::getForce(void)
{
    return force;
}

/***************************************************
 * Methos gets mass for particle
 **************************************************/
float Particle::getMass(void)
{
    return mass;
}

/***************************************************
 * Method returns bool true/false based on whether 
 * the particle can/can not move
 **************************************************/
bool Particle::canMove(Vector p, vector<Particle> particles, int exclude)
{
    bool can_move = true;
    
    if (p.y < groundLevel)
    {
        //Ground bounces back the particle with a velocity in y equal to initial * elasticity factor
        can_move = false;
    }
    else
    {
        Particle colliding_particle;
        
        int particle_count = particles.size();
        for (int j = 0; j < particle_count; ++j)
        {
            if (j != exclude)
            {
                Particle temp_particle = particles[j];
                
                Vector diffVt = p - temp_particle.getPosition();
                
                if (diffVt.magnitude() <= radius+temp_particle.getRadius())
                {
                    can_move = false;
                    break;
                }
            }
        }
    }
    
    return can_move;
}

/***************************************************
 * Method returns radius for the particle
 **************************************************/
float Particle::getRadius()
{
    return radius;
}

/***************************************************
 * Destructor
 **************************************************/
Particle::~Particle(void) {}


