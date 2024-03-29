/**
 * @file pworld.h
 * 
 * @brief Keeps track of a set of particles, and provides the means to
 *        update them all.
 */

#pragma once


#include "particle.h"
#include "pcontacts.h"
#include "pfgen.h"

#include <Gorgon/Geometry/Point.h>

#include <Gorgon/Containers/Collection.h>

namespace Gorgon
{
    namespace Physics
    {
        /**
         * Particle World represents the whole physically simulated world.
         * The World will be created at the beginning of the game and
         * destroyed upon ending the game.
         */
        
        /// inherit  from animation
        class ParticleWorld
        {
        public:
            /// *** Colliction 
            /// Where all particles in the world stored;
//             typedef std::vector<Particle*> Particles; 
            
            /// Where all contact generators in the world stored;
//             typedef std::vector<ParticleContactGenerator*> ContactGenerators;
        protected:

            /**
             * Holds all particles 
             */
            Gorgon::Containers::Collection<Particle> particles;

            /**
             * Holds all contact generators
            */
            Gorgon::Containers::Collection<ParticleContactGenerator> contactGens;

            /**
             * Holds the force generators for the particle in this world 
             */
            ParticleForceRegistry registry;

            /**
            * Holds the resolver for contacts.
            */
            ParticleContactResolver resolver;

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
             * Deletes the physically simulated world
            */
           ~ParticleWorld();

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
            * This method it calls all force generator to 
            * apply their forces and then perform the intergration,
            * also runs the contact the detector and resolve contacts.
            */
            void RunPhysics(unsigned time);

            /**
            * Returns the list of contact generators.
            */
            inline Gorgon::Containers::Collection<ParticleContactGenerator>& GetContactGens(){
                return contactGens;
            };

            /**
             *  Returns the list of particles.
             */
            Gorgon::Containers::Collection<Particle>& GetParticles();

            /**
            * Returns the force registry.
            */
            ParticleForceRegistry& GetForceRegistry();
        };


        /**
         * Ground contact generator that takes a vector of 
         * particles and collides them against the ground
         */
        class GroundContacts : public ParticleContactGenerator
        {
        protected:
            Gorgon::Containers::Collection<Particle> particles;
//             ParticleWorld::Particles *particles;

        public:
            void init(Containers::Collection<Particle> &particle);

            virtual unsigned AddContact(ParticleContact *contact, unsigned limit) const;
        };
    }
}
