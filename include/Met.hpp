
#ifndef Met_hpp
#define Met_hpp

#include "Particle.hpp"

class Met : public Particle {
    
public:
    Met();
    Met(double pt, double phi);
    Met(const TLorentzVector&);
    virtual ~Met();
    void set(double pt, double phi);    
};

#endif