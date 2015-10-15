
#include "VariableDictionary.hpp"

VariableDictionary::VariableDictionary(const EventContainer& evContainer)
: eventContainer(evContainer) { }

VariableDictionary::~VariableDictionary(){ }

void VariableDictionary::stringToFunction(const vector<string>& variableNames, vector<eventContainerFunction>& eventFunctions)
{
    vector<ComparisonType> temp;
    stringToFunction(variableNames, eventFunctions, temp);
}

void VariableDictionary::stringToFunction(const vector<string>& variableNames, vector<eventContainerFunction>& eventFunctions, vector<ComparisonType>& ComparisonTypes)
{
    for( const string& iString : variableNames )
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
                eventFunctions.push_back( bind(&EventContainer::jetpt, &eventContainer, getIndex(iSubString, iString, true)) );
                if( maxFlag )
                    ComparisonTypes.push_back( ComparisonType::SMALLER_THAN );
                else
                    ComparisonTypes.push_back( ComparisonType::GREATER_THAN );
            }
            else if( iSubString.find("eta") != string::npos ) 
            {
                string::size_type varPosition = iSubString.find("eta");
                iSubString.erase(varPosition, 3);
                eventFunctions.push_back( bind(&EventContainer::jeteta, &eventContainer, getIndex(iSubString, iString, true)) );
                if( minFlag )
                    ComparisonTypes.push_back( ComparisonType::ABS_GREATER_THAN );
                else
                    ComparisonTypes.push_back( ComparisonType::ABS_SMALLER_THAN );
            }
            else if( iSubString.find("mass") != string::npos ) 
            {
                string::size_type varPosition = iSubString.find("mass");
                iSubString.erase(varPosition, 4);
                eventFunctions.push_back( bind(&EventContainer::jetmass, &eventContainer, getIndex(iSubString, iString, true)) );
                if( maxFlag )
                    ComparisonTypes.push_back( ComparisonType::SMALLER_THAN );
                else
                    ComparisonTypes.push_back( ComparisonType::GREATER_THAN );
            }
            else
            {
                cerr << "Variable '" << iString << "' in cuts or variables list not known." << endl;
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
                eventFunctions.push_back( bind(&EventContainer::leptonpt, &eventContainer, getIndex(iSubString, iString, false)) );
                if( maxFlag )
                    ComparisonTypes.push_back( ComparisonType::SMALLER_THAN );
                else
                    ComparisonTypes.push_back( ComparisonType::GREATER_THAN );
            }
            else if( iSubString.find("eta") != string::npos ) 
            {
                string::size_type varPosition = iSubString.find("eta");
                iSubString.erase(varPosition, 3);
                eventFunctions.push_back( bind(&EventContainer::leptoneta, &eventContainer, getIndex(iSubString, iString, false)) );
                if( minFlag )
                    ComparisonTypes.push_back( ComparisonType::ABS_GREATER_THAN );
                else
                    ComparisonTypes.push_back( ComparisonType::ABS_SMALLER_THAN );
            }
            else
            {
                cerr << "Variable '" << iString << "' in cuts or variables list not known." << endl;
                throw 1;
            }
        }
        
        // Double particle variables
        else if( iSubString == "mll" ) 
        {
            eventFunctions.push_back( bind(&EventContainer::mll, &eventContainer) );
            if( maxFlag )
                ComparisonTypes.push_back( ComparisonType::SMALLER_THAN );
            else
                ComparisonTypes.push_back( ComparisonType::GREATER_THAN );
        }
        else if( iSubString == "mjj" ) 
        {
            eventFunctions.push_back( bind(&EventContainer::mjj, &eventContainer) );
            if( maxFlag )
                ComparisonTypes.push_back( ComparisonType::SMALLER_THAN );
            else
                ComparisonTypes.push_back( ComparisonType::GREATER_THAN );
        }
        else if( iSubString == "ptll" ) 
        {
            eventFunctions.push_back( bind(&EventContainer::ptll, &eventContainer) );
            if( maxFlag )
                ComparisonTypes.push_back( ComparisonType::SMALLER_THAN );
            else
                ComparisonTypes.push_back( ComparisonType::GREATER_THAN );        
        }
        else if( iSubString == "ptjj" ) 
        {
            eventFunctions.push_back( bind(&EventContainer::ptjj, &eventContainer) );
            if( maxFlag )
                ComparisonTypes.push_back( ComparisonType::SMALLER_THAN );
            else
                ComparisonTypes.push_back( ComparisonType::GREATER_THAN ); 
        }
        else if( iSubString == "detall" ) 
        {
            eventFunctions.push_back( bind(&EventContainer::detall, &eventContainer) );
            if( maxFlag )
                ComparisonTypes.push_back( ComparisonType::ABS_SMALLER_THAN );
            else
                ComparisonTypes.push_back( ComparisonType::ABS_GREATER_THAN ); 
        }
        else if( iSubString == "detajj" ) 
        {
            eventFunctions.push_back( bind(&EventContainer::detajj, &eventContainer) );
            if( maxFlag )
                ComparisonTypes.push_back( ComparisonType::ABS_SMALLER_THAN );
            else
                ComparisonTypes.push_back( ComparisonType::ABS_GREATER_THAN ); 
        }
        else if( iSubString == "dphill" ) 
        {
            eventFunctions.push_back( bind(&EventContainer::dphill, &eventContainer) );
            if( maxFlag )
                ComparisonTypes.push_back( ComparisonType::ABS_SMALLER_THAN );
            else
                ComparisonTypes.push_back( ComparisonType::ABS_GREATER_THAN ); 
        }
        else if( iSubString == "dphijj" ) 
        {
            eventFunctions.push_back( bind(&EventContainer::dphijj, &eventContainer) );
            if( maxFlag )
                ComparisonTypes.push_back( ComparisonType::ABS_SMALLER_THAN );
            else
                ComparisonTypes.push_back( ComparisonType::ABS_GREATER_THAN ); 
        }
        
        // Special variables
        else if( iSubString == "channel" ) 
        {
            eventFunctions.push_back( bind(&EventContainer::channel, &eventContainer) );
            ComparisonTypes.push_back( ComparisonType::EQUAL );
        } 
        
        else
        {
            cerr << "Variable '" << iString << "' in cuts or variables list not known." << endl;
            throw 1;
        }
        cout << "Found variable: " << iString << endl;
    }
}

unsigned int VariableDictionary::getIndex(const string& indexString, const string& fullString, bool isJet)
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