/**
 * @file plinks.h
 * @brief Links connect two particles together, generating a contact if
 * they violate the constraints of their link. It is used as a
 * base class for cables and rods, and could be used as a base
 * class for springs with a limit to their extension.
 */

#include <Gorgon/Physics/particle.h>
#include <Gorgon/Physics/pcontacts.h>
#include <Gorgon/Geometry/Point.h>

namespace Gorgon
{
    namespace Physics
    {

        /**
         * Links interface
         */
        class ParticleLinks
        {
        public:
            /**
             *  Holds pair of particles that are connected by this link
             */
            Particle *particle[2];

            /**
             * Fills the given contact structure with the contact needed
             * to keep the link from violating its constraint. The contact
             * pointer should point to the first available contact in a
             * contact array, where @param limit is the maximum number of
             * contacts in the array that can be written to. The method
             * returns the number of contacts that have been written. This
             * format is common to contact-generating functions, but this
             * class can only generate a single contact, so the
             * pointer can be a pointer to a single element. The limit
             * parameter is assumed to be at least one (zero isn’t valid),
             * and the return value is either 0, if the cable wasn’t
             * overextended, or one if a contact was needed.
             */

            virtual unsigned AddContact(ParticleContact *contact, unsigned limit) const = 0;

        protected:
            /**
             *  Returns the current length of the link
             */
            inline double CurrentLength() const;
        };


        /**
         * Cables links a pair of particles and generates 
         * a contact if they stray too far. 
         */

        class CableLink : ParticleLinks
        {
        private:

            /**
             * Holds the maximum length of a cable
             */
            double maxLength;

            /**
             * Holds the restitution for the cable 
             */
            double restitution;

        public:
           /**
            * Fills the given contact structure with the contact needed
            * to keep the cable from over-extending.
            */

           virtual unsigned AddContact(ParticleContact *contact, unsigned limit) const;
        };

        /**
         * Rods links a pair of particles and generates 
         * a contact if they stray too far. 
         */

        class RodLink : ParticleLinks
        {
        private:
            /**
             * Holds the length of the rod
             */
            double length;

        public:
            /**
            * Fills the given contact structure with the contact needed
            * to keep the rod from extending or compressing.
            */
            virtual unsigned AddContact(ParticleContact *contact, unsigned limit) const;
        };
    }
}