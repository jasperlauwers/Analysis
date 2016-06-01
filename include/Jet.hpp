
#ifndef Jet_hpp
#define Jet_hpp

#include "Particle.hpp"

class Jet : public Particle {
    
public:
    Jet();
    Jet(float pt, float eta, float phi, float mass = 0, float puId = 0, float qgLikelihood = 0, float CSV = 0);
    Jet(const TLorentzVector&, float puId = 0, float qgLikelihood = 0, float CSV = 0);
    virtual ~Jet();
    void set(float pt, float eta, float phi, float mass, float puId, float qgLikelihood, float CSV);
    void set(float pt, float eta, float phi, float mass = 0);
    void setCsv( float csv );
    void setPuId( float puId );
    void setIsHardProcess( float isHardProcess );
    float csv() const;
    float puId() const;
    bool isHardProcess() const;
    
private:
    float _puId, qgLikelihood, CSV;
    bool _isHardProcess; 
    
};

#endif