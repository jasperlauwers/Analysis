#ifndef EventSelecter_hpp
#define EventSelecter_hpp

#include <iostream>
#include <string>
#include <functional>
#include "EventContainer.hpp"
#include "CutContainer.cpp"

using namespace std;

typedef function<double ()> eventContainerFunction;
typedef function<bool ()> comparisonFunction;

enum class ComparisonType { GREATER_THAN, SMALLER_THAN, ABS_GREATER_THAN, ABS_SMALLER_THAN, EQUAL };

class EventSelecter {
public:
    EventSelecter(EventContainer&, CutContainer&);
    ~EventSelecter();
    bool passCuts();
    bool passCut(unsigned int);
    
private:
    unsigned int getIndex(const string& indexString, const string& fullString, bool isJet);
    bool greaterThan(int index);
    bool smallerThan(int index);
    bool absGreaterThan(int index);
    bool absSmallerThan(int index);
    bool equal(int index);
    
    EventContainer& eventContainer;
    const CutContainer& cutContainer;
    vector<eventContainerFunction> functionVector;
    vector<ComparisonType> comparisonTypeVector;
    vector<comparisonFunction> fullComparisonVector;
    
};
#endif