#include <Gorgon/Physics/pworld.h>

using Gorgon::Physics::ParticleWorld;

ParticleWorld::ParticleWorld(unsigned maxContacts, unsigned iterations)
: resolver(iterations), 
maxContacts(maxContacts)
{
    contacts = new ParticleContact[maxContacts];
    calculateIterations = (iterations == 0);
}

ParticleWorld::~ParticleWorld()
{
    delete[] contacts;
}

void ParticleWorld::StartFrame()
{
    /// Loop over all particles in the world
    for(Particles::iterator itr = particles.begin(); 
        itr != particles.end();
        itr++
       )
    {
        (*itr)->ClearAccumulator();
    }
}

unsigned ParticleWorld::GenerateContacts()
{
    unsigned limit = maxContacts;
    ParticleContact *nextContact = contacts;

    /// Loop over all contact generators
    for(ContactGenerators::iterator itr = contactGens.begin();
        itr != contactGens.end();
        itr++)
    {
        unsigned used = (*itr)->AddContact(nextContact, limit);
        limit -= used;
        nextContact += used;

        if(limit <= 0) break;
    }

    /// Returns the number of contacts used
    return maxContacts - limit;
}

void ParticleWorld::Integrate(unsigned time)
{
    for(Particles::iterator itr = particles.begin();
        itr != particles.end();
        itr++)
    {
        (*itr)->Integrator(time);
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