
#include "Met.hpp"

Met::Met() { }

Met::Met(float pt, float phi) 
    : Particle( pt, 0, phi, 0) { }

Met::Met(const TLorentzVector& p4) 
: Particle( p4 ) { }

Met::~Met() { }

void Met::set(float pt, float phi) 
{
    Particle::set( pt, 0, phi, 0);
}