/**
 * @file pfgen.cpp the implementation of the force generators
 * @author Ahmad Bader (ahmadqasem.b@gmail.com)
 */

#include "./pfgen.h"

using Gorgon::Geometry::Point3D;
using Gorgon::Physics::Particle;
using Gorgon::Physics::ParticleForceGenerator;
using Gorgon::Physics::ParticleForceRegistry;
using Gorgon::Physics::GravityGenerator;
using Gorgon::Physics::SpringGenerator;
using Gorgon::Physics::SpringAnchorGenerator;
using Gorgon::Physics::BungeeGenerator;

/********************************************************************
 * Particle Force Registry Class Implementation
********************************************************************/

void ParticleForceRegistry::Add(Particle * particle, ParticleForceGenerator *fg)
{
    // creates a new registry, and assign to it the particle and the fg
    // and then push back to the list of registrations
    ParticleForceRegistry::ParticleForceRegistration registration;
    registration.particle = particle;
    registration.fg = fg;
    registrations.push_back(registration);
}

void ParticleForceRegistry::UpdateForces(double time)
{
    //loop over all the fg and update all the forces
    Registry::iterator itr = registrations.begin();
    for(; itr != registrations.end(); itr++)
    {   
        itr->fg->UpdateForce(itr->particle, time);
    }
}


/********************************************************************
* Gravity Force Generator Class Implementation
********************************************************************/

GravityGenerator::GravityGenerator(const Point3D &gravity)
: gravity(gravity)
{
}

void GravityGenerator::UpdateForce(Particle *particle, double time)
{
    //don't generate gravity if the particle is massless or has infinite mass
    if(particle->HasFiniteMass()) return;

    // apply the mass-scaled force on the particle
    particle->AddForce(gravity * particle->GetMass());
}


/********************************************************************
 * Spring and Spring-Like Force Generators Class Implementation
********************************************************************/

SpringGenerator::SpringGenerator(Particle &other, double spring_constant, double rest_length)
    : other(other), spring_constant(spring_constant), rest_length(rest_length)
{
}

void SpringGenerator::UpdateForce(Particle *particle, double time)
{

    //calculating the vector of the spring
    Point3D force;
    force = particle->GetPosition();
    force = force - other.GetPosition();

    //calculating the magnitude of the force
    double magnitude = force.Distance(); 
    magnitude = std::abs(magnitude - rest_length);
    magnitude *= spring_constant;

    //calculate the final force and then apply it
    force.Normalize();
    force = force * (-magnitude);
    particle->AddForce(force);
}

SpringAnchorGenerator::SpringAnchorGenerator()
{

};

SpringAnchorGenerator::SpringAnchorGenerator(Point3D &other, double sc, double rl)
    : anchor(other), spring_constant(sc), rest_length(rl)
{ };

void SpringAnchorGenerator::UpdateForce(Particle *particle, double time)
{

    //calculating the vector of the spring
    Point3D force;
    force = particle->GetPosition();
    force = force - anchor;

    //calculating the magnitude of the spring
    double magintude = force.Distance();
    magintude = (rest_length - magintude) * spring_constant;

    //calculating the final force and then apply it
    force.Normalize();
    force = force * magintude;
    particle->AddForce(force);
}

BungeeGenerator::BungeeGenerator(Particle &other, double sc, double rl)
: other(other), springConstant(sc), restLength(rl)
{
}

void BungeeGenerator::UpdateForce(Particle *particle, double time)
{
    //calculating the vector of the spring
    Point3D force;
    force = particle->GetPosition();
    force = force - other.GetPosition();

    //check if the bungee is compressed
    double magintude = force.Distance();
    if(magintude <= restLength) return;

    //calculating the magintude of the force
    magintude = springConstant * (restLength - magintude);

    //calculate the final force and then apply it
    force.Normalize();
    force = force * (-magintude);
    particle->AddForce(force);
}
