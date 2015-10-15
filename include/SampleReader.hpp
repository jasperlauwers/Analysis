
#ifndef SampleReader_hpp
#define SampleReader_hpp

#include "SampleContainer.hpp"
#include "ConfigReader.hpp"

#include <vector>
#include <iostream>

class SampleReader : public ConfigReader {
public:
    SampleReader(const string& fileName, SampleContainer& sampleContainer);
    virtual ~SampleReader();
    
private:
    SampleContainer& sampleContainer;
    
};
#endif