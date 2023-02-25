/**
 * @file particle.cpp contains the implementation for the Particle Class
 * @author Ahmad Bader (ahmadqasem.b@gmail.com)
 */
#include "particle.h"
#include <assert.h>
#include <limits>

using Gorgon::Geometry::Point;
using Gorgon::Physics::Particle;

void Particle::integrator(double time)
{
    // don't integrate particles with zero mass
    if (inverseMass <= 0.0f)
        return;

    // abort the program if the time elapsed
    // between frames is less than zero
    assert(time > 0);

    /**
     * Update linear position
     * we don't pay attiention to the accleration in the position update
     * because the resulting acceleration would be very small due
     * the smale time intervals so it wouldn't have impact,
     * we will take it into consideration in the velocity update
     */

    /// TODO create a function to add a scaled vector directly, ex addScaledVector(vecotr, scale)
    position += velocity * time; 
    

    // work out the acceleratino from the applied force
    Point resultingAcceleration = acceleration;

    // add to the resulting acceleration force scaled with the inverse mass
    resultingAcceleration += forceAccum * inverseMass;

    // update the velcity
    velocity += resultingAcceleration * time;

    // impose damping (drag)
    // impose drag by each frame instead of per intervals of time.
    velocity *= pow(damping, time);

    // clear the accumulated forces
    clearAccumulator();
};

void Particle::setMass(const double mass)
{
    assert(mass != 0);
    Particle::inverseMass = (1.0f / mass);
};

double Particle::getMass() const
{   
    // if the inverse mass is zero, that means it has infinite mass
    if(inverseMass == 0)
    {
        return std::numeric_limits<double>::max();
    }
    else
    {
        return (1.0f / inverseMass);
    }
};

void Particle::setInverseMass(const double inverseMass)
{
    Particle::inverseMass = inverseMass;
};

double Particle::getInverseMass() const
{
    return Particle::inverseMass;
};

void Particle::setDamping(const double damping)
{
    Particle::damping = damping;
};

double Particle::getDamping() const
{
    return Particle::damping;
};

bool Particle::hasFiniteMass() const
{
    return inverseMass >= 0.0f;
};

// TODO --> setPosition using x, y parameters
void Particle::setPosition(const Point &position)
{
    Particle::position.X = position.X;
    Particle::position.Y = position.Y;
    //Particle::position.Z = position.Z;
};

// TODO --> getPosition that return a pointer to the position
Point Particle::getPosition() const 
{
    return Particle::position;
};

void Particle::setVelocity(const Point &velocity)
{
    Particle::velocity.X = velocity.X;
    Particle::velocity.Y = velocity.Y;
    //Particle::velocity.Z = velocity.Z;
};

// TODO --> getPosition that return a pointer to the velocity
Point Particle::getVelocity() const 
{
    return Particle::velocity;
};

void Particle::setAcceleration(const Point &acceleration)
{
    Particle::acceleration = acceleration;
};

// TODO --> getAcceleration that return a pointer to the acceleration
Point Particle::getAcceleration() const
{
    return Particle::acceleration;
};

void Particle::clearAccumulator()
{
    forceAccum.X = forceAccum.Y = 0;
};

void Particle::addForce(const Point &force)
{
    forceAccum += force;
};