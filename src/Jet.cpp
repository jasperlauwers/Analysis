
#include "Jet.hpp"

Jet::Jet() { }

Jet::Jet(float pt, float eta, float phi, float mass, float puId, float qgLikelihood, float CSV) 
    : Particle( pt, eta, phi, mass), _puId(puId), _qgLikelihood(qgLikelihood), CSV(CSV) { }

Jet::Jet(const TLorentzVector& p4, float puId, float qgLikelihood, float CSV) 
: Particle( p4 ), _puId(puId), _qgLikelihood(qgLikelihood), CSV(CSV) { }

Jet::~Jet() { }

void Jet::set(float pt, float eta, float phi, float mass, float puId, float qgLikelihood, float CSV) 
{
    Particle::set( pt, eta, phi, mass);
    _puId = puId;
    _qgLikelihood = qgLikelihood;
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
void Jet::setPuId( float puId )
{
    _puId = puId;
}
void Jet::setQgLikelihood( float qg )
{
    _qgLikelihood = qg;
}
void Jet::setIsHardProcess( float isHardProcess )
{
    _isHardProcess = isHardProcess;
}
// void Jet::setSoftMuPt( float softMuPt )
// {
//     softMuPt;
// }
void Jet::setSoftMu( float pt, float eta, float phi )
{
    softMuon = Lepton(pt, eta, phi, 13);
}
float Jet::csv() const
{
    return CSV;
}
float Jet::puId() const
{
    return _puId;
}
float Jet::qgLikelihood() const
{
    return _qgLikelihood;
}
bool Jet::isHardProcess() const
{
    return _isHardProcess;
}
float Jet::softMuPt() const
{
    return softMuon.pt();
}
float Jet::softMuDr() const
{
    return this->dR(softMuon);
}