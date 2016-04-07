
#ifndef VariableReader_hpp
#define VariableReader_hpp

#include "VariableContainer.hpp"
#include "ConfigReader.hpp"

#include <vector>
#include <iostream>

class VariableReader : public ConfigReader {
public:
    VariableReader(const string& fileName, VariableContainer& varContainer);
    virtual ~VariableReader();
    
private:
    void readVariableName(const Setting&);
    VariableContainer& variableContainer;
    bool is2D;    
};
#endif