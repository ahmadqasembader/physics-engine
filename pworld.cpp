#include <Gorgon/Physics/pworld.h>

using Gorgon::Physics::ParticleWorld;

void ParticleWorld::StartFrame()
{
    ParticleRegistration *reg = firstParticle;

    while(reg)
    {
        /// Remove all forces from the accumulator
        reg->particle->ClearAccumulator();

        /// Get the next registration
        reg = reg->next;
    }
}

unsigned ParticleWorld::GenerateContacts()
{
    unsigned limit = maxContacts;
    ParticleContact *nextContact = contacts;

    ContactGenRegistration *reg = firstContactGen;
    while(reg)
    {
        unsigned used = reg->gen->addContact(nextContact, limit);
        limit -= used;
        nextContact += used;

        /// We’ve run out of contacts to fill. This means we’re missing contacts
        if(limit <= 0) break;

        reg = reg->next;
    }

    /// Returns the number of contacts used
    return maxContacts - limit;
}

void ParticleWorld::Integrate(unsigned time)
{
    ParticleRegistration *reg = firstParticle;

    while(reg)
    {
        /// Remove all forces from the force accumulator
        reg->particle->ClearAccumulator();

        /// Get the next registration
        reg = reg->next;
    }
}

void ParticleWorld::RunPhysics(unsigned time)
{
    /// First apply the forces generators
    registry.UpdateForces(time);

    /// Then integrate the object
    Integrate(time);

    /// Generate contacts
    unsigned usedContacts = GenerateContacts();

    /// Process these contacts
    if(usedContacts)
    {
        if(calculateIterations)
        {
            resolver.SetIterations(usedContacts * 2);
        }
        resolver.ResolveContacts(contacts, usedContacts, time);
    }
}