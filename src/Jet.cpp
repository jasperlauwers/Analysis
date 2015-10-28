
#include "Jet.hpp"

Jet::Jet() { }

Jet::Jet(double pt, double eta, double phi, double mass, double puId, double qgLikelihood, double CSV) 
    : Particle( pt, eta, phi, mass), puId(puId), qgLikelihood(qgLikelihood), CSV(CSV) { }

Jet::Jet(const TLorentzVector& p4, double puId, double qgLikelihood, double CSV) 
: Particle( p4 ), puId(puId), qgLikelihood(qgLikelihood), CSV(CSV) { }

Jet::~Jet() { }

void Jet::set(double pt, double eta, double phi, double mass, double puId, double qgLikelihood, double CSV) 
{
    Particle::set( pt, eta, phi, mass);
    this->puId = puId;
    this->qgLikelihood = qgLikelihood;
    this->CSV = CSV;
}

void Jet::set(double pt, double eta, double phi, double mass) 
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