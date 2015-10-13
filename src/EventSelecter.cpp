
#include "EventSelecter.hpp"

EventSelecter::EventSelecter(EventContainer& evContainer, CutContainer& cContainer)
: eventContainer(evContainer), cutContainer(cContainer)
{
    for( const string& iString : cutContainer.variableNames )
    {
        string iSubString = iString;
        // Check if there is a min or max flag
        bool minFlag = false, maxFlag = false;
        
        string::size_type minPosition = iSubString.find("min");
        if( minPosition != string::npos )
        {
            minFlag = true;
            iSubString.erase(minPosition, 3);
        }
        
        string::size_type maxPosition = iSubString.find("max");
        if( maxPosition != string::npos )
        {
            maxFlag = true;
            iSubString.erase(maxPosition, 3);
        }

        // Jet variables
        if( iSubString.find("jet") != string::npos )
        {
            string::size_type jetPosition = iSubString.find("jet");
            iSubString.erase(jetPosition, 3);

            if( iSubString.find("pt") != string::npos ) 
            {
                string::size_type varPosition = iSubString.find("pt");
                iSubString.erase(varPosition, 2);
                functionVector.push_back( bind(&EventContainer::jetpt, &eventContainer, getIndex(iSubString, iString, true)) );
                if( maxFlag )
                    comparisonTypeVector.push_back( ComparisonType::SMALLER_THAN );
                else
                    comparisonTypeVector.push_back( ComparisonType::GREATER_THAN );
            }
            else if( iSubString.find("eta") != string::npos ) 
            {
                string::size_type varPosition = iSubString.find("eta");
                iSubString.erase(varPosition, 3);
                functionVector.push_back( bind(&EventContainer::jeteta, &eventContainer, getIndex(iSubString, iString, true)) );
                if( minFlag )
                    comparisonTypeVector.push_back( ComparisonType::ABS_GREATER_THAN );
                else
                    comparisonTypeVector.push_back( ComparisonType::ABS_SMALLER_THAN );
            }
            else if( iSubString.find("mass") != string::npos ) 
            {
                string::size_type varPosition = iSubString.find("mass");
                iSubString.erase(varPosition, 4);
                functionVector.push_back( bind(&EventContainer::jetmass, &eventContainer, getIndex(iSubString, iString, true)) );
                if( maxFlag )
                    comparisonTypeVector.push_back( ComparisonType::SMALLER_THAN );
                else
                    comparisonTypeVector.push_back( ComparisonType::GREATER_THAN );
            }
            else
            {
                cerr << "Variable '" << iString << "' in cuts list not known." << endl;
                throw 1;
            }
        }
    
        // Lepton variables
        else if( iSubString.find("lepton") != string::npos )
        {
            string::size_type leptonPosition = iSubString.find("lepton");
            iSubString.erase(leptonPosition, 6);
            
            if( iSubString.find("pt") != string::npos ) 
            {
                string::size_type varPosition = iSubString.find("pt");
                iSubString.erase(varPosition, 2);
                functionVector.push_back( bind(&EventContainer::leptonpt, &eventContainer, getIndex(iSubString, iString, false)) );
                if( maxFlag )
                    comparisonTypeVector.push_back( ComparisonType::SMALLER_THAN );
                else
                    comparisonTypeVector.push_back( ComparisonType::GREATER_THAN );
            }
            else if( iSubString.find("eta") != string::npos ) 
            {
                string::size_type varPosition = iSubString.find("eta");
                iSubString.erase(varPosition, 3);
                functionVector.push_back( bind(&EventContainer::leptoneta, &eventContainer, getIndex(iSubString, iString, false)) );
                if( minFlag )
                    comparisonTypeVector.push_back( ComparisonType::ABS_GREATER_THAN );
                else
                    comparisonTypeVector.push_back( ComparisonType::ABS_SMALLER_THAN );
            }
            else
            {
                cerr << "Variable '" << iString << "' in cuts list not known." << endl;
                throw 1;
            }
        }
        
        // Double particle variables
        else if( iSubString == "mll" ) 
        {
            functionVector.push_back( bind(&EventContainer::mll, &eventContainer) );
            if( maxFlag )
                comparisonTypeVector.push_back( ComparisonType::SMALLER_THAN );
            else
                comparisonTypeVector.push_back( ComparisonType::GREATER_THAN );
        }
        else if( iSubString == "mjj" ) 
        {
            functionVector.push_back( bind(&EventContainer::mjj, &eventContainer) );
            if( maxFlag )
                comparisonTypeVector.push_back( ComparisonType::SMALLER_THAN );
            else
                comparisonTypeVector.push_back( ComparisonType::GREATER_THAN );
        }
        else if( iSubString == "ptll" ) 
        {
            functionVector.push_back( bind(&EventContainer::ptll, &eventContainer) );
            if( maxFlag )
                comparisonTypeVector.push_back( ComparisonType::SMALLER_THAN );
            else
                comparisonTypeVector.push_back( ComparisonType::GREATER_THAN );        
        }
        else if( iSubString == "ptjj" ) 
        {
            functionVector.push_back( bind(&EventContainer::ptjj, &eventContainer) );
            if( maxFlag )
                comparisonTypeVector.push_back( ComparisonType::SMALLER_THAN );
            else
                comparisonTypeVector.push_back( ComparisonType::GREATER_THAN ); 
        }
        else if( iSubString == "detall" ) 
        {
            functionVector.push_back( bind(&EventContainer::detall, &eventContainer) );
            if( maxFlag )
                comparisonTypeVector.push_back( ComparisonType::ABS_SMALLER_THAN );
            else
                comparisonTypeVector.push_back( ComparisonType::ABS_GREATER_THAN ); 
        }
        else if( iSubString == "detajj" ) 
        {
            functionVector.push_back( bind(&EventContainer::detajj, &eventContainer) );
            if( maxFlag )
                comparisonTypeVector.push_back( ComparisonType::ABS_SMALLER_THAN );
            else
                comparisonTypeVector.push_back( ComparisonType::ABS_GREATER_THAN ); 
        }
        else if( iSubString == "dphill" ) 
        {
            functionVector.push_back( bind(&EventContainer::dphill, &eventContainer) );
            if( maxFlag )
                comparisonTypeVector.push_back( ComparisonType::ABS_SMALLER_THAN );
            else
                comparisonTypeVector.push_back( ComparisonType::ABS_GREATER_THAN ); 
        }
        else if( iSubString == "dphijj" ) 
        {
            functionVector.push_back( bind(&EventContainer::dphijj, &eventContainer) );
            if( maxFlag )
                comparisonTypeVector.push_back( ComparisonType::ABS_SMALLER_THAN );
            else
                comparisonTypeVector.push_back( ComparisonType::ABS_GREATER_THAN ); 
        }
        
        // Special variables
        else if( iSubString == "channel" ) 
        {
            functionVector.push_back( bind(&EventContainer::channel, &eventContainer) );
            comparisonTypeVector.push_back( ComparisonType::EQUAL );
        } 
        
        else
        {
            cerr << "Variable '" << iString << "' in cuts list not known." << endl;
            throw 1;
        }
        cout << "Added cut variable: " << iString << endl;
    }
    
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

bool EventSelecter::passCuts()
{
    for( const auto& iFunc : fullComparisonVector )
    {
        if( !iFunc() ) return false;
    }
    return true;
}

bool EventSelecter::passCut(unsigned int i) // make <=, >, abs<=, abs> functions and bind with cut values
{
    if( i < fullComparisonVector.size() ) // remove this for speed?
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

bool EventSelecter::greaterThan(int index)
{
    return (functionVector[index]() > cutContainer.cutValues[index]);
}
bool EventSelecter::smallerThan(int index)
{
    return (functionVector[index]() <= cutContainer.cutValues[index]);
}
bool EventSelecter::absGreaterThan(int index)
{
    return (abs(functionVector[index]()) > cutContainer.cutValues[index]);
}
bool EventSelecter::absSmallerThan(int index)
{
    return (abs(functionVector[index]()) <= cutContainer.cutValues[index]);
}
bool EventSelecter::equal(int index)
{
    return (functionVector[index]() == cutContainer.cutValues[index]);
}