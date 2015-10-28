#ifndef VariableDictionary_hpp
#define VariableDictionary_hpp

#include <iostream>
#include <string>
#include <functional>
#include "EventContainer.hpp"

using namespace std;

typedef function<float ()> eventContainerFunction;

enum class ComparisonType { GREATER_THAN, SMALLER_THAN, ABS_GREATER_THAN, ABS_SMALLER_THAN, EQUAL };

class VariableDictionary {
public:
    VariableDictionary(const EventContainer&);
    ~VariableDictionary();
    void stringToFunction(const vector<string>&, vector<eventContainerFunction>&) const;
    void stringToFunction(const vector<string>&, vector<eventContainerFunction>&, vector<ComparisonType>&) const;
    
private:
    unsigned int getIndex(const string& indexString, const string& fullString, bool isJet) const;   
    float getFloat(const string& indexString, const string& fullString) const; 
    const EventContainer& eventContainer;    
};
#endif