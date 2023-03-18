/**
 * @file pcontacts.cpp is implementation for pcontact.h
 * 
 */

#include "pcontacts.h"
using Gorgon::Geometry::Point;
using Gorgon::Physics::Particle;
using Gorgon::Physics::ParticleContact;


void ParticleContact::Resolve(unsigned long time)
{
    ResolveVelocity(time);
}

double ParticleContact::CalcSepVel() const
{
    /**
     * We can calculate the seperating velocity using the
     * equation: Vs = (velocity_A - velocity_B) * contactNormal(the direction)
     */
    Point relativeVelocity = particle[0].GetVelocity();

    //check if there's another object
    if(&particle[1])
        relativeVelocity -= particle[1].GetVelocity();

    return (relativeVelocity * ContactNormal);
}

void ParticleContact::ResolveVelocity(unsigned long time)
{
    /**
     * Get the seperating velocity before collision
     * Here we are using the seperating velocity instead of 
     * the closing velocity. They're same values, just flip their signs.
    */
    double sepVel = CalcSepVel();

    /**
     * If the seperating velocity greater than zero it
     * means that the closing velocity is less than zero
     * which means the two objects are moving away
     * from each other (Not on a collision trajectory).
     */
    if(sepVel > 0) return;


    /**
     * Calculate the seperating velocity after collision
     * using the formula Vs (After) = Vs (before) * restitution
     */
    double newSepVel = -sepVel * restitution;

    // Remember, an impulse is the total change in the velocity
    double deltaVelocity = newSepVel - sepVel;

    /**
     * We apply the change in velocity to each object
     * in proportion to its inverse mass 
     * (less inverse mass = higher actual mass, which get less change in velocity)
     */

    double totalInverseMass = particle[0].GetInverseMass();
    if(&particle[1])
        totalInverseMass += particle[1].GetInverseMass();
    
    // Impulse have no effects if the objects have infinite masses
    if(totalInverseMass <= 0) return;

    // Calculate the impulse
    double impulse = deltaVelocity / totalInverseMass;

    // Find the amount of impulses per unit of inverse mass
    Point impulsePerMass = ContactNormal * impulse;

    // Apply the all impulses
    particle[0].SetVelocity(particle[0].GetVelocity() + impulsePerMass * particle[0].GetInverseMass());

    if(&particle[1])
    {
        // The 2nd particle goes into the opposite direction 
        particle[1].SetVelocity(particle[1].GetVelocity() + impulsePerMass * -particle[1].GetInverseMass());
    }
}