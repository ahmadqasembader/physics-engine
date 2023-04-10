/**
 * @file pworld.h
 * 
 * @brief Keeps track of a set of particles, and provides the means to
 *        update them all.
 */

#pragma once

#include <Gorgon/Physics/particle.h>
#include <Gorgon/Physics/pcontacts.h>
#include <Gorgon/Physics/pfgen.h>
#include <Gorgon/Geometry/Point.h>

namespace Gorgon
{
    namespace Physics
    {
        class ParticleWorld
        {
        protected:
            /**
             * Holds one particle in the linked list of all particles 
             */
            struct ParticleRegistration
            {
                Particle *particle;
                ParticleRegistration *next;
            };

            /**
             * Holds a list of particle registration 
             */
            ParticleRegistration *firstParticle;
        
            /**
             * Holds the force generators for the particle in this world 
             */
            ParticleForceRegistry registry;

            /**
            * Holds the resolver for contacts.
            */
            ParticleContactResolver resolver;

            /**
             * Holds the registered contact generator 
             */

            struct ContactGenRegistration
            {
                ParticleContactGenerator *gen;
                ContactGenRegistration *next;
            };

            /**
             * Holds the list of contact generators 
             */
            ContactGenRegistration *firstContactGen;

            /**
             * Holds a list of contacts 
             */
            ParticleContact *contacts;

            /**
             * Holds the maximum number of contacts allowed 
             */
            unsigned maxContacts;

            /**
             * True if the world should calculate the number of iterations
             * to give the contact resolver at each frame.
             */
            bool calculateIterations;
            
        public:
            /**
            * Creates a new particle simulator that can handle up to the
            * given number of contacts per frame. You can also optionally
            * give a number of contact-resolution iterations to use. If you
            * don’t give a number of iterations, then twice the number of
            * contacts will be used.
            */
           ParticleWorld(unsigned maxContacts, unsigned iterations = 0);

           /**
            * Initializes the world for a simulation frame. This clears
            * the force accumulators for particles in the world. After
            * calling this, the particles can have their forces for this
            * frame added.
            */
           void StartFrame();

           /**
            * Calls each of the registered contact generators to report
            * their contacts. Returns the number of generated contacts.
            */
            unsigned GenerateContacts();

            /**
            * Integrates all the particles in this world forward in time
            * by the given duration.
            */
            void Integrate(unsigned time);

            /**
            * Processes all the physics for the particle world.
            */
            void RunPhysics(unsigned time);
        };
    }
}