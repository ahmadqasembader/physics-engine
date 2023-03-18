/**
 * @file pcontacts.h
 * @brief Interface file for the contact resolution system for particles.
 * 
 */

#include <Gorgon/Physics/particle.h>
#include <Gorgon/Geometry/Point.h>
#include <Gorgon/Geometry/Point3D.h>

namespace Gorgon
{
    namespace Physics
    {
        class ParticleContact
        {
            public:
                // Hold the two colliding/contacting particles
                // The second index can be NULL if the second object is a scenery or immovale object
                Particle particle[2];

                // Holds the normal restitution coefficient at the contact.
                double restitution;

                // Holds the direction of the contact 
                Gorgon::Geometry::Point ContactNormal;
            
            protected:
                // A central function resolve contacts and interpenetration
                void Resolve(unsigned long time);

                // Calculate the seperating velocity at this contact
                double CalcSepVel() const;

            private:
                // Calculates the impulse for this contact
                void ResolveVelocity(unsigned long time);
        };
    }
}