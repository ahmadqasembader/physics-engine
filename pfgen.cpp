/**
 * @file pfgen.cpp the implementation of the force generators
 * @author Ahmad Bader (ahmadqasem.b@gmail.com)
 */

#include "pfgen.h"

using Gorgon::Geometry::Point;
using Gorgon::Physics::Particle;
using Gorgon::Physics::ParticleForceGenerator;
using Gorgon::Physics::ParticleForceRegistry;

void ParticleForceRegistry::Add(Particle * particle, ParticleForceGenerator *fg)
{
    // creates a new registry, and assign to it the particle and the fg
    // and then push back to the list of registrations
    ParticleForceRegistry::ParticleRegistration registration;
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