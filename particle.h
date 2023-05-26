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
#include <assert.h>
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

            inline void SetMass(const double value){
                assert(value != 0);
                inverseMass = (1.0f / value);
            };
            inline double GetMass() const{
                // if the inverse mass is zero, that means it has infinite mass
                if (inverseMass == 0){
                    return std::numeric_limits<double>::max();
                } else{
                    return (1.0f / inverseMass);
                }
            };

            inline void SetInverseMass(const double value){
                inverseMass = value;
            };
            inline double GetInverseMass() const{
                return this->inverseMass;
            };

            inline void SetDamping(const double value){
                damping = value;
            };
            inline double GetDamping() const{
                return this->damping;
            };

            inline void SetPosition(const Point &value){
                position = value;
            };
            inline void SetPosition(const int &x, const int &y){
                this->position.X = x;
                this->position.Y = y;
            }
            inline Point GetPosition() const{
                return this->position;
            };

            inline void SetVelocity(const Point &value){
                velocity = value;
            };
            inline Point GetVelocity() const{
                return this->velocity;
            };

            inline void SetAcceleration(const Point &value){
                acceleration = value;
            };
            inline Point GetAcceleration() const{
                return this->acceleration;
            };

            inline void ClearAccumulator(){
                forceAccum.X = forceAccum.Y = 0;
            };
            inline void AddForce(const Point &force){
                forceAccum += force;
            };
            inline bool HasFiniteMass() const{
                return (inverseMass >= 0.0f);
            };
        };  
    }
}
