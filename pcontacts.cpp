/**
 * @file pcontacts.cpp is implementation for pcontact.h
 * 
 */

#include "pcontacts.h"
using Gorgon::Geometry::Point3D;
using namespace Gorgon::Physics;

void ParticleContact::Resolve(unsigned long time)
{
    ResolveVelocity(time);
    ResolveInterPenetration(time);
}

double ParticleContact::CalcSepVel() const
{
    /**
     * We can calculate the seperating velocity using the
     * equation: Vs = (velocity_A - velocity_B) * contactNormal
     */
    Point3D relativeVelocity = particle[0]->GetVelocity();

    //check if there's another object
    if(particle[1] != nullptr)
        relativeVelocity = relativeVelocity - particle[1]->GetVelocity();

    //return the seperating velocity along the contact normal
    return (relativeVelocity * ContactNormal);
}

void ParticleContact::ResolveVelocity(unsigned long time)
{

    /**
     * The goal in this function is to calculate and apply the 
     * magintude of the impulse in the direction of the contact normal.
     * 
     * To achieve that, we'll apply the following steps
     * 
     *  1. Calculate the seperating veloicty pre-collision
     *  2. Calculate the seperating veloicty post-collision
     *  3. Calculate the delta seperating velocity 
     *  4. Calculate the total inverse mass
     *  5. Calculate the magintude of the impulse:
     *  6. Calculate the impulse with respect to the contact normal
     *  7. Apply the impulse using the following formula: 
     *
     * 
     *  Note: Here we are using the seperating velocity instead of 
     *     the closing velocity. It's a matter of preference only.
    */  


    // Get the seperating velocity before collision
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

            // Check the velocity build-up due to acceleration only.
            Point3D accCausedVelocity = particle[0]->GetAcceleration();
            if(particle[1] != nullptr)
                accCausedVelocity = accCausedVelocity - particle[1]->GetAcceleration();

            double accCausedSepVelocity = accCausedVelocity * ContactNormal * time;

            // If we've got a closing velocity due to acceleration build-up,
            // remove it from the new separating velocity
            if (accCausedSepVelocity < 0)
            {
                newSepVel += restitution * accCausedSepVelocity;

                // Make sure we haven't removed more than was
                // there to remove.
                if (newSepVel < 0) newSepVel = 0;
            }

    /**
     * Remember, an impulse is the total change in the velocity, 
     * therefore, we calculate the new relative velocity.
     * The new relative velocity is the delta velocity (pre and post collision)
     * */ 

    double deltaVelocity = newSepVel - sepVel;

    /**
     * We apply the change in velocity to each object
     * in proportion to its inverse mass 
     * (less inverse mass = higher actual mass, which get less change in velocity)
     */

    double totalInverseMass = particle[0]->GetInverseMass();
    if(particle[1] != nullptr)
        totalInverseMass += particle[1]->GetInverseMass();
    
    // Impulse have no effects if the objects have infinite masses
    if(totalInverseMass <= 0) return;

    // The magintude of the impulse
    double impulse = deltaVelocity / totalInverseMass;
 
    /*
     * Find the amount of impulse per unit of inverse mass.
     * Remember: impulse at essence is a velocity, however, it is
     * a scaler quantity, that's why we calculate the contact normal seperatly.
     */

    Point3D impulsePerMass = ContactNormal * impulse;

    /**
     * Apply the all impulses. They're applied in the 
     * direction of the contact normal and the impulses are 
     * proportional to the inverse mass.
     * 
     * We use the following equation: 
     *          NewVelocity = OldVelocity + InverseMass * TotalImpulses
     */

    particle[0]->SetVelocity(particle[0]->GetVelocity() + impulsePerMass * particle[0]->GetInverseMass());

    if(particle[1] != nullptr)
    {
        // The 2nd particle goes into the opposite direction,
        // hence the nigative sign to the inverse mass
        particle[1]->SetVelocity(particle[1]->GetVelocity() + impulsePerMass * -particle[1]->GetInverseMass());
    }
}

void ParticleContact::ResolveInterPenetration(unsigned long time)
{
    // If there's no penetration, exit;
    if (penetration <= 0) return;

    // The movement of the objects are in propertion with their mass    
    double totalInverseMass = particle[0]->GetInverseMass();
    if(particle[1] != nullptr) 
        totalInverseMass += particle[1]->GetInverseMass();

    // If the objects that are colliding have infinite masses, we do nothing
    if(totalInverseMass <= 0) return;

    // Find the amount of penetration per unit of inverse mass
    Point3D movePerIMass = ContactNormal * (-penetration / totalInverseMass);

    // Apply the penetration
    particle[0]->SetPosition(particle[0]->GetPosition() + movePerIMass * particle[0]->GetInverseMass());
    if(particle[1] != nullptr)
        particle[1]->SetPosition(particle[1]->GetPosition() + movePerIMass * particle[1]->GetInverseMass());

}


ParticleContactResolver::ParticleContactResolver(unsigned iterations)
: iterations(iterations)  {};

// void ParticleContactResolver::SetIterations(unsigned iterations)
// {
//     this->iterations = iterations;
// }

void ParticleContactResolver::ResolveContacts(ParticleContact *contactArr, unsigned numOfContacts, double time)
{
    unsigned i;
    iterationsUsed = 0;

    while (iterationsUsed < iterations)
    {
        //Find the contact with the largest closing velocity
        double max = std::numeric_limits<double>::max();

        unsigned maxIndex = numOfContacts;

        for (i = 0; i < numOfContacts; i++)
        {
            double sepVel = contactArr[i].CalcSepVel();

            if(sepVel < max)
            {
                max = sepVel;
                maxIndex = i;
            }
        }
        // Do we have anything worth resolving?
        if (maxIndex == numOfContacts) break;

        // Resolve this contact
        contactArr[maxIndex].Resolve(time);

        iterationsUsed++;
        
    }
    
}
