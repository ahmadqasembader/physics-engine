/**
 * @file plinks.h
 * @brief Links connect two particles together, generating a contact if
 * they violate the constraints of their link. It is used as a
 * base class for cables and rods, and could be used as a base
 * class for springs with a limit to their extension.
 */
#pragma once

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

        /**
         * Constraints are just like links, except they connect a particle to
         * an immovable anchor point.
         *
         * This class will act as interface for
         * links' constraints (cable and rods constraints)
         */
        class Constraint : public ParticleContactGenerator
        {
        protected:
            /**
             * Returns the current length of the link
             */
            double CurrentLength() const;

        public:
            /**
             * Holds the particle that are connected to the
             * anchor point
             */
            Particle *particle

                /**
                 * Holds the anchor point dimisions
                 */
                Point *anchor

                /**
                 * Fills the given contact structure with the generated
                 * contact. The contact pointer should point to the first
                 * available contact in a contact array, where limit is the
                 * maximum number of contacts in the array that can be written
                 * to. The method returns the number of contacts that have
                 * been written.
                 */
                virtual unsigned
                AddContact(ParticleContact *contact,
                           unsigned limit) const = 0;
        }

        /**
         * Cables link a particle to an anchor point, generating a contact if they
         * stray too far apart.
         */

        class CableConstraints : public Constraint
        {
        public:
            /**
             * Holds the maximum length of the cable.
             */
            double maxLength;

            /**
             * Holds the restitution (bounciness) of the cable.
             */
            double restitution;

            /**
             * Fills the given contact structure with the contact needed
             * to keep the cable from over-extending.
             */
            virtual unsigned AddContact(ParticleContact *contact,
                                        unsigned limit) const;
        }

        /**
         * Rods link a particle to an anchor point, generating a contact if they
         * stray too far apart or too close.
         */
        class RodConstraints : Constraint
        {
        public:
            /**
             * Holds the fixed length of the rod
             */
            double length;

            virtual unsigned AddConntact(ParticleContact *contact,
                                         unsigned limit) const;
        }
    }

}