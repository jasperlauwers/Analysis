
#include "EventSelecter.hpp"

EventSelecter::EventSelecter(const EventContainer& evContainer, const CutContainer& cContainer)
: eventContainer(evContainer), cutContainer(cContainer)
{
    // Set functions to variables and comparison types
    VariableDictionary varDictionary(eventContainer);
    varDictionary.stringToFunction(cutContainer.variableNames, functionVector, comparisonTypeVector);
    
    // Now bind the full comparison together
    for( unsigned int i = 0; i < functionVector.size(); ++i )
    {
        if( comparisonTypeVector[i] == ComparisonType::GREATER_THAN )
        {
            fullComparisonVector.push_back( bind(&EventSelecter::greaterThan, this, i) );
        }
        else if( comparisonTypeVector[i] == ComparisonType::SMALLER_THAN )
        {
            fullComparisonVector.push_back( bind(&EventSelecter::smallerThan, this, i) );
        }
        else if( comparisonTypeVector[i] == ComparisonType::ABS_GREATER_THAN )
        {
            fullComparisonVector.push_back( bind(&EventSelecter::absGreaterThan, this, i) );
        }
        else if( comparisonTypeVector[i] == ComparisonType::ABS_SMALLER_THAN )
        {
            fullComparisonVector.push_back( bind(&EventSelecter::absSmallerThan, this, i) );
        }
        else if( comparisonTypeVector[i] == ComparisonType::EQUAL )
        {
            fullComparisonVector.push_back( bind(&EventSelecter::equal, this, i) );
        }
    }
}

EventSelecter::~EventSelecter(){ }

bool EventSelecter::passCuts() const
{
    for( const auto& iFunc : fullComparisonVector )
    {
        if( !iFunc() ) return false;
    }
    return true;
}

bool EventSelecter::passCut(unsigned int i) const // make <=, >, abs<=, abs> functions and bind with cut values
{
    if( i < fullComparisonVector.size() )
    {
        return fullComparisonVector[i]();
    }
    else
    {
        cerr << "Call to passCut with argument " << i << " out of bounds." << endl;
        throw 1;
    }
}

unsigned int EventSelecter::getIndex(const string& indexString, const string& fullString, bool isJet)
{
    if( indexString.size() != 1 )
    {
        cerr << "Missing index for variable: " << fullString << endl;
        throw 1;
    }
    
    unsigned int index = (unsigned) stoi(indexString);
    
    if( index == (unsigned) 0 )
    {
        cerr << "Index 0 for variable: " << fullString << endl;
        throw 1;        
    }
    
    // jet1 = jet with index 0
    index--;
    return index;    
}

bool EventSelecter::greaterThan(int index) const
{
    return (functionVector[index]() >= cutContainer.cutValues[index]);
}
bool EventSelecter::smallerThan(int index) const
{
    return (functionVector[index]() <= cutContainer.cutValues[index]);
}
bool EventSelecter::absGreaterThan(int index) const
{
    return (abs(functionVector[index]()) > cutContainer.cutValues[index]);
}
bool EventSelecter::absSmallerThan(int index) const
{
    return (abs(functionVector[index]()) <= cutContainer.cutValues[index]);
}
bool EventSelecter::equal(int index) const
{
    return ( abs(functionVector[index]() - cutContainer.cutValues[index]) < 1e-5 );
}