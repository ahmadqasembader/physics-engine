/**
 * @file particle.cpp contains the implementation for the Particle Class
 * @author Ahmad Bader (ahmadqasem.b@gmail.com)
 */
#include <Gorgon/Physics/particle.h>
#include <Gorgon/Utils/Assert.h>
// #include <assert.h>
#include <limits>

using Gorgon::Geometry::Point3D;
using Gorgon::Physics::Particle;

void Particle::Integrator(unsigned long time)
{
    // don't integrate particles with zero mass
    if (inverseMass <= 0.0f)
        return;

    // abort the program if the time elapsed
    // between frames is less than zero
    if(time < 0.0)
        throw std::runtime_error("time cannot be less than zero");

    /**
     * Update linear position
     * we don't pay attiention to the accleration in the position update
     * because the resulting acceleration would be very small due
     * the smale time intervals so it wouldn't have impact,
     * we will take it into consideration in the velocity update
     */

    /// TODO create a function to add a scaled vector directly, ex addScaledVector(vecotr, scale)
    position = position + (velocity * time);

    // work out the acceleratino from the applied force
    Point3D resultingAcceleration = acceleration;

    // add to the resulting acceleration force scaled with the inverse mass
    resultingAcceleration = resultingAcceleration + (forceAccum * inverseMass);

    // update the velcity
    velocity = velocity + (resultingAcceleration * time);

    // impose damping (drag)
    // impose drag by each frame instead of per intervals of time.
    velocity = velocity * pow(damping, time);

    // clear the accumulated forces
    ClearAccumulator();
};

// void Particle::SetMass(const double value)
// {
//     assert(value != 0);
//     inverseMass = (1.0f / value);
// };

// double Particle::GetMass() const
// {
//     // if the inverse mass is zero, that means it has infinite mass
//     if (inverseMass == 0)
//     {
//         return std::numeric_limits<double>::max();
//     }
//     else
//     {
//         return (1.0f / inverseMass);
//     }
// };

// void Particle::SetInverseMass(const double value)
// {
//     inverseMass = value;
// };

// double Particle::GetInverseMass() const
// {
//     return this->inverseMass;
// };

// void Particle::SetDamping(const double value)
// {
//     damping = value;
// };
// 
// double Particle::GetDamping() const
// {
//     return this->damping;
// };

// bool Particle::HasFiniteMass() const
// {
//     return (inverseMass >= 0.0f);
// };

// void Particle::SetPosition(const Point3D &value)
// {
//     this->position = value;
// };

// void Particle::SetPosition(const int &x, const int &y)
// {
//     this->position.X = x;
//     this->position.Y = y;
// }

// Point3D Particle::GetPosition() const
// {
//     return this->position;
// };
// 
// void Particle::SetVelocity(const Point3D &value)
// {
//     this->velocity = value;
// };
// 
// Point3D Particle::GetVelocity() const
// {
//     return this->velocity;
// };
// 
// void Particle::SetAcceleration(const Point3D &value)
// {
//     acceleration = value;
// };
// 
// Point3D Particle::GetAcceleration() const
// {
//     return this->acceleration;
// };
// 
// void Particle::ClearAccumulator()
// {
//     forceAccum.X = forceAccum.Y = 0;
// };
// 
// void Particle::AddForce(const Point3D &force)
// {
//     forceAccum += force;
// };
