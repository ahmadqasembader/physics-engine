/**
 * @file particle.h contains Particle class
 * @author Ahmad Bader (ahmadqasem.b@gmail.com)
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
#pragma once

#include <Gorgon/Geometry/Point.h>
#include <Gorgon/Geometry/Point3D.h>

using Gorgon::Geometry::Point;
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

            /**
             * this variable holds the accumulated force that to
             * be applied in the next frame update.
             * it's also cleared by each frame.
             */
            Point forceAccum;

        public:
            /*
             * This function performs mathematical integration
             * by working out the acceleration from the force
             * and working out the velocity with respect to
             * the acceleration and then update the position
             * with the velocity
             */
            void Integrator(unsigned long time);

            inline void SetMass(const double mass);
            inline double GetMass() const;

            inline void SetInverseMass(const double inverseMass);
            inline double GetInverseMass() const;

            inline void SetDamping(const double damping);
            inline double GetDamping() const;

            inline void SetPosition(const Point &position);
            inline void SetPosition(const int &x, const int &y);
            inline Point GetPosition() const;

            inline void SetVelocity(const Point &velocity);
            inline Point GetVelocity() const;

            inline void SetAcceleration(const Point &acceleration);
            inline Point GetAcceleration() const;

            inline void ClearAccumulator();
            inline void AddForce(const Point &force);
            inline bool HasFiniteMass() const;
        };  
    }
}
