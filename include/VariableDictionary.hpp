#ifndef VariableDictionary_hpp
#define VariableDictionary_hpp

#include <iostream>
#include <string>
#include <functional>
#include "EventContainer.hpp"

using namespace std;

typedef function<double ()> eventContainerFunction;

enum class ComparisonType { GREATER_THAN, SMALLER_THAN, ABS_GREATER_THAN, ABS_SMALLER_THAN, EQUAL };

class VariableDictionary {
public:
    VariableDictionary(const EventContainer&);
    ~VariableDictionary();
    void stringToFunction(const vector<string>&, vector<eventContainerFunction>&);
    void stringToFunction(const vector<string>&, vector<eventContainerFunction>&, vector<ComparisonType>&);
    
private:
    unsigned int getIndex(const string& indexString, const string& fullString, bool isJet);
    
    const EventContainer& eventContainer;    
};
#endif