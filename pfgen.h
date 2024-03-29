/**
 * @file pfgen.h contains the particle force generator interface and the force generators
 * @author Ahmad Bader (ahmadqasem.b@gmail.com)
 *
 *
 * @brief the particle force generator is an interface that has
 * an updateForce method that has to be implemented by each force generator
 * This way, the engine abstract the complexity and the calculations for each
 * force from the particles. The generator will simply performe all the calculations
 * and will call the addForce method that is attached to the particle, and this way,
 * the engine can attach many particles to the same force generator since they
 * don't keep any partile related data.
 *
 *
 * @version 0.1
 * @date 2023-03-04
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once

#include <Gorgon/Physics/particle.h>
#include <Gorgon/Geometry/Point3D.h>
#include <vector>

using Gorgon::Geometry::Point3D;
namespace Gorgon
{
    namespace Physics
    {

        // The particle force generator interface
        class ParticleForceGenerator
        {
        public:
            /**
             * overload this method in each force generator
             * to calculate, update and performe the force-specific calculations
             */
            virtual void UpdateForce(Gorgon::Physics::Particle *particle, double time) = 0;
        };

        /**
         * A force generator that applies gravity on the 
         * supplied particle 
         */
        class GravityGenerator : public ParticleForceGenerator
        {
            Point3D gravity;
        public: 
            // It creates a gravity generator with the given gravity
            GravityGenerator(const Point3D &gravity);

            virtual void UpdateForce(Particle *particle, double time);
        };


        class SpringGenerator : public ParticleForceGenerator
        {
            Particle other;

            double spring_constant;

            double rest_length;

        public:
            SpringGenerator(Particle &particle, double spring_constant, double rest_length);

            virtual void UpdateForce(Particle *particle, double time);
        };

        class SpringAnchorGenerator : public ParticleForceGenerator
        {
            Point3D anchor;

            double spring_constant;

            double rest_length;

        public: 
            SpringAnchorGenerator();

            SpringAnchorGenerator(Point3D &anchor, double spring_constant, double rest_length);

            const Point3D GetAnchor() const {return anchor;}

            void init(Point3D &anchor, double spring_constant, double rest_length);

            virtual void UpdateForce(Particle *Particle, double time);
        };


             
        class BungeeGenerator : public ParticleForceGenerator
        {
            
            Particle other;

            double springConstant;

            double restLength;

        public:

            BungeeGenerator(Particle &other,
                double springConstant, double restLength);

            virtual void UpdateForce(Particle *particle, double duration);
        };

        /**
         * Holds all the force generators and the particles they apply to.
         */
        class ParticleForceRegistry
        {
        protected:
            /**
             * This struct is the represenation of each force registration.
             * It keeps track with one force and its particle and the attached
             * force generator.
             */
            struct ParticleForceRegistration
            {
                Gorgon::Physics::Particle *particle;
                ParticleForceGenerator *fg;
            };

            typedef std::vector<ParticleForceRegistration> Registry;
            Registry registrations;

        public:
            /**
             * It creates and new ParticleRegistration and store it
             * to keep track with it
             */
            void Add(Gorgon::Physics::Particle *particle, ParticleForceGenerator *fg);

            /**
             * It calls all the force generators and
             * it updates attached particles' forces
             */
            void UpdateForces(double time);
        };
    }
}
