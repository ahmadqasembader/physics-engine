/**
 * @file pcontacts.h
 * @brief Interface file for the contact resolution system for particles.
 *
 */
#pragma once

#include <limits>

#include <Gorgon/Physics/particle.h>
#include <Gorgon/Geometry/Point3D.h>
namespace Gorgon {
    namespace Physics
    {
        class ParticleContact
        {

        friend class ParticleContactResolver;

        public:
            // Hold the two colliding/contacting particles
            // The second index can be NULL if the second object is a scenery or immovale object
            // std::array
            Particle *particle[2];

            // Holds the normal restitution coefficient at the contact.
            double restitution;

            // Holds the direction of the contact
            Gorgon::Geometry::Point3D ContactNormal;

            // Holds the depth of the penetration
            double penetration;

        protected:
            // A central function resolve contacts and interpenetration
            void Resolve(unsigned long time);

            // Calculate the seperating velocity at this contact
            double CalcSepVel() const;

        private:
            // Calculates the impulse for this contact
            void ResolveVelocity(unsigned long time);

            void ResolveInterPenetration(unsigned long time);
        };
        class ParticleContactResolver
        {
        protected:
            // Holds the number of iterations allowed.
            unsigned iterations;

            // A value to keep track with the actual number of iterations used
            unsigned iterationsUsed;

        public:
            // Creates a new contact resolver
            ParticleContactResolver(unsigned iterations);

            inline void SetIterations(unsigned iterations){
                this->iterations = iterations;
            }

            /**
             *
             * @param contactsArr array of particle contact objects (i.e. objects that are contacting)
             * @param numOfContacts the number of the contacts that happened in the contact array
             * @param time time passed since the last integration (i.e. the last frame)
             */
            void ResolveContacts(ParticleContact *contactArr, unsigned numOfContacts, double time);
        };
        /**
         * This is the basic interface for contact generators
         * applying to particles.
         */

        class ParticleContactGenerator
        {
        public:
            /**
             * Fills the given contact structure with the generated
             * contact. The contact pointer should point to the first
             * available contact in a contact array, where limit is the
             * maximum number of contacts in the array that can be written
             * to. The method returns the number of contacts that have
             * been written.
             */
            virtual unsigned AddContact(ParticleContact *contact, unsigned limit) const = 0;
        };
    };

}
