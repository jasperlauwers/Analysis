
#ifndef FakeReader_hpp
#define FakeReader_hpp

#include "FakeContainer.hpp"
#include "ConfigReader.hpp"

#include <vector>
#include <iostream>

class FakeReader : public ConfigReader {
public:
    FakeReader(const string& fileName, FakeContainer& fakeContainer);
    virtual ~FakeReader();
    
private:
    FakeContainer& fakeContainer;
    
};
#endif