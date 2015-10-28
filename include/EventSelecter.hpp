#ifndef EventSelecter_hpp
#define EventSelecter_hpp

#include <iostream>
#include <string>
#include <functional>
#include "EventContainer.hpp"
#include "CutContainer.hpp"
#include "VariableDictionary.hpp"

using namespace std;

typedef function<float ()> eventContainerFunction;
typedef function<bool ()> comparisonFunction;

// enum class ComparisonType { GREATER_THAN, SMALLER_THAN, ABS_GREATER_THAN, ABS_SMALLER_THAN, EQUAL }; // defined in VariableDictionary

class EventSelecter {
public:
    EventSelecter(const EventContainer&, const CutContainer&);
    ~EventSelecter();
    bool passCuts() const;
    bool passCut(unsigned int) const;
    
private:
    unsigned int getIndex(const string& indexString, const string& fullString, bool isJet);
    bool greaterThan(int index) const;
    bool smallerThan(int index) const;
    bool absGreaterThan(int index) const;
    bool absSmallerThan(int index) const;
    bool equal(int index) const;
    
    const EventContainer& eventContainer;
    const CutContainer& cutContainer;
    vector<eventContainerFunction> functionVector;
    vector<ComparisonType> comparisonTypeVector;
    vector<comparisonFunction> fullComparisonVector;
    
};
#endif