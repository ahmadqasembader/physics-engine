#include "plinks.h"

using Gorgon::Geometry::Point;
using Gorgon::Physics::RodLink;
using Gorgon::Physics::Particle;
using Gorgon::Physics::CableLink;
using Gorgon::Physics::ParticleLinks;
using Gorgon::Physics::ParticleContact;

double ParticleLinks::CurrentLength() const
{
    Point relativePos = particle[0]->GetPosition() 
                            -  particle[1]->GetPosition();

    return relativePos.Distance();
};

unsigned CableLink::AddContact(ParticleContact *contact, unsigned limit) const
{
    /// Get the current length
    double length = CurrentLength();

    /// Check if the cable is overextended
    if(length < maxLength) return 0;

    contact->particle[0] = particle[0];
    contact->particle[1] = particle[1];

    /// calculate the contact normal
    Point normal = particle[1]->GetPosition() - particle[0]->GetPosition();
    normal.Normalize();
    contact->ContactNormal = normal;

    contact->penetration = length - maxLength;
    contact->restitution = restitution;

    return 1;
}

unsigned RodLink::AddContact(ParticleContact *contact, unsigned limit) const
{
    /// Get the current length of the rod
    double currlength = CurrentLength();

    /// Check if the cable is overextended
    if(currlength == length) return 0;

    contact->particle[0] = particle[0];
    contact->particle[1] = particle[1];

    /// Calculate the contact normal
    Point normal = particle[1]->GetPosition() - particle[0]->GetPosition();
    normal.Normalize();

    /// The contact normal depends on whether we're extending or compressing
    if (currlength > length) {
        contact->ContactNormal = normal;
        contact->penetration = currlength - length;
    } else {
        contact->ContactNormal = normal * -1;
        contact->penetration = length - currlength;
    }

    /// Always use zero restitution
    contact->restitution = 0;

    return 1;
}