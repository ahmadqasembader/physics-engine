#include "pworld.h"

using namespace Gorgon::Physics;
using namespace Gorgon::Containers;
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
    
    for(Particle &p : particles){
        p.ClearAccumulator();
    }
    
    /// Loop over all particles in the world
//     for(Particles::iterator itr = particles.begin(); 
//         itr != particles.end();
//         itr++
//        )
//     {
//         (*itr)->ClearAccumulator();
//     }
}

unsigned ParticleWorld::GenerateContacts()
{
    unsigned limit = maxContacts;
    ParticleContact *nextContact = contacts;
    
    for(ParticleContactGenerator &gen : contactGens){
        unsigned used = gen.AddContact(nextContact, limit);
        limit -= used;
        nextContact += used;
        if (limit<=0) break;
    }
        
    
    /// Loop over all contact generators
//     for(ContactGenerators::iterator itr = contactGens.begin();
//         itr != contactGens.end();
//         itr++)
//     {
//         unsigned used = (*itr)->AddContact(nextContact, limit);
//         limit -= used;
//         nextContact += used;
// 
//         if(limit <= 0) break;
//     }

    /// Returns the number of contacts used
    return maxContacts - limit;
}

void ParticleWorld::Integrate(unsigned time)
{
    
    for(Particle &p : particles){
        p.Integrator(time);
    }
    
    /*for(Particles::iterator itr = particles.begin();
        itr != particles.end();
        itr++)
    {
        (*itr)->Integrator(time);
    }*/
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

// Collection<ParticleContactGenerator>& ParticleWorld::GetContactGens(){
//     return contactGens;
// }

Collection<Particle> &ParticleWorld::GetParticles(){
    return particles;
}

// ParticleWorld::Particles& ParticleWorld::GetParticles()
// {
//     return particles;
// }

void GroundContacts::init(Containers::Collection<Particle> &particle)
{
//     GroundContacts::particles = particle;
}

unsigned GroundContacts::AddContact(ParticleContact *contact, unsigned limit) const
{
    unsigned count = 0;
    Point3D UP(0, 1, 0);
    for(Particle &p : particles){
        double y = p.GetPosition().Y;
        if(y<0.0f){
            contact->ContactNormal = UP;
            contact->particle[0] = &p;
            contact->particle[1] = NULL;
            contact->penetration = -y;
            contact->restitution = 0.2f;
            contact++;
            count++;
        }
        if(count >= limit) 
        {
            return count;
        }
    }
//     for (ParticleWorld::Particles::iterator itr = particles->begin(); 
//     itr != particles->end();
//     itr++)
//     {
//         double y = (*itr)->GetPosition().Y;
//         if(y < 0.0f)
//         {
//             //contact->ContactNormal = Point3D::UP;
//             contact->particle[0] = *itr;
//             contact->particle[1] = NULL;
//             contact->penetration = -y;
//             contact->restitution = 0.2f;
//             contact++;
//             count++;
//         }
//         
//         if(count >= limit) 
//         {
//             return count;
//         }
//     }

    return count;
}



ParticleForceRegistry& ParticleWorld::GetForceRegistry(){
    return registry;
}
