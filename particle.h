/**
 * @file particle.h contains Particle class
 * @author Ahmad Bader (ahmadqasem.b@google.com)
 *
 *
 * @brief This class defines what is a particle
 * and defines its properties and utility functions a particle might need
 *
 *
 * @version 0.1
 * @date 2023-02-24
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <Gorgon/Geometry/Point.h>
#include <Gorgon/Geometry/Point3D.h>
using Gorgon::Geometry::Point;
using Gorgon::Geometry::Point3D;

namespace Gorgon
{
    namespace Physics
    {
        class Particle
        {
        protected:
            double inverseMass;
            double damping;

            Point position;
            Point velocity;
            Point acceleration;
            Point position;
            Point forceAccum;

        public:
            /*
             * This function performs mathematical integration
             * by working out the acceleration from the force
             * and working out the velocity with respect to
             * the acceleration and then update the position
             * with the velocity
             */
            void integrator(double time);

            void setMass(const double mass);
            double getMass() const;

            void setInverseMass(const double inverseMass);
            double getInverseMass() const;

            void setDamping(const double damping);
            double getDamping() const;

            void setPosition(const Point &position);
            Point getPosition() const;

            void setVelocity(const Point &velocity);
            Point getVelocity() const;

            void setAcceleration(const Point &acceleration);
            Point getAcceleration() const;

            void clearAccumulator();
            void addForce(const Point &force);
            bool hasFiniteMass() const;
        };
    }
}
