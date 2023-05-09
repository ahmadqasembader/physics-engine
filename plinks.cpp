#include "plinks.h"

using Gorgon::Geometry::Point;
using Gorgon::Physics::RodLink;
using Gorgon::Physics::Particle;
using Gorgon::Physics::CableLink;
using Gorgon::Physics::Constraint;
using Gorgon::Physics::ParticleLinks;
using Gorgon::Physics::ParticleContact;

using Gorgon::Physics::RodConstraints;
using Gorgon::Physics::CableConstraints;

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

double Constraint::CurrentLength() const
{
    Point relativePos = particle->GetPosition() - anchor;
    return relativePos.Distance();
};

unsigned CableConstraints::AddContact(ParticleContact *contact,
                unsigned limit) const
{
    /// Find the current length
    double length = CurrentLength();

    /// Check if we're over-extended or not
    if(length < maxLength) return 0;

    /// Otherwise return the contact
    contact->particle[0] = particle;
    contact->particke[1] = 0;

    /// Calculate the contact normal (collision direction)
    Point normal = anchor - particle->GetPosition(); 
    normal.Normalize();
    contact->ContactNormal = normal;

    contact->restitution = restitution;
}


unsigned RodConstraints::addContact(ParticleContact *contact,
                                 unsigned limit) const
{
    // Find the length of the rod
    double currentLen = CurrentLength();

    // Check if we're over-extended
    if (currentLen == length) return 0;

    // Otherwise return the contact
    contact->particle[0] = particle;
    contact->particle[1] = 0;

    // Calculate the normal
    Point normal = anchor - particle->getPosition();
    normal.Normalise();

    // The contact normal depends on whether we're extending or compressing
    if (currentLen > length) {
        contact->ContactNormal = normal;
    } else {
        contact->contactNormal = normal * -1;
    }

    // Always use zero restitution (no bounciness)
    contact->restitution = 0;

    return 1;
}