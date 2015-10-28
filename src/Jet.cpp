
#include "Jet.hpp"

Jet::Jet() { }

Jet::Jet(float pt, float eta, float phi, float mass, float puId, float qgLikelihood, float CSV) 
    : Particle( pt, eta, phi, mass), puId(puId), qgLikelihood(qgLikelihood), CSV(CSV) { }

Jet::Jet(const TLorentzVector& p4, float puId, float qgLikelihood, float CSV) 
: Particle( p4 ), puId(puId), qgLikelihood(qgLikelihood), CSV(CSV) { }

Jet::~Jet() { }

void Jet::set(float pt, float eta, float phi, float mass, float puId, float qgLikelihood, float CSV) 
{
    Particle::set( pt, eta, phi, mass);
    this->puId = puId;
    this->qgLikelihood = qgLikelihood;
    this->CSV = CSV;
}

void Jet::set(float pt, float eta, float phi, float mass) 
{
    Particle::set(pt, eta, phi, mass);
}

void Jet::setCsv( float csv )
{
    CSV = csv;
}
float Jet::csv() const
{
    return CSV;
}