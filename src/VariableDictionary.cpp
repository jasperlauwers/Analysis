
#include "VariableDictionary.hpp"

VariableDictionary::VariableDictionary(const EventContainer& evContainer)
: eventContainer(evContainer) { }

VariableDictionary::~VariableDictionary(){ }

void VariableDictionary::stringToFunction(const vector<string>& variableNames, vector<eventContainerFunction>& eventFunctions) const
{
    vector<ComparisonType> temp;
    stringToFunction(variableNames, eventFunctions, temp);
}

void VariableDictionary::stringToFunction(const vector<string>& variableNames, vector<eventContainerFunction>& eventFunctions, vector<ComparisonType>& ComparisonTypes) const
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
        if( iSubString.find("njets") != string::npos ) 
        {
            string::size_type jetPosition = iSubString.find("njets");
            iSubString.erase(jetPosition, 5);
            eventFunctions.push_back( bind(&EventContainer::nJets, &eventContainer, getFloat(iSubString, iString)) );
            if( maxFlag )
                ComparisonTypes.push_back( ComparisonType::SMALLER_THAN );
            else
                ComparisonTypes.push_back( ComparisonType::GREATER_THAN );
        }
        else if( iSubString.find("genjet") != string::npos )
        {
            string::size_type jetPosition = iSubString.find("genjet");
            iSubString.erase(jetPosition, 6);

            if( iSubString.find("pt") != string::npos ) 
            {
                string::size_type varPosition = iSubString.find("pt");
                iSubString.erase(varPosition, 2);
                eventFunctions.push_back( bind(&EventContainer::genjetpt, &eventContainer, getIndex(iSubString, iString)) );
                if( maxFlag )
                    ComparisonTypes.push_back( ComparisonType::SMALLER_THAN );
                else
                    ComparisonTypes.push_back( ComparisonType::GREATER_THAN );
            }
            else if( iSubString.find("eta") != string::npos ) 
            {
                string::size_type varPosition = iSubString.find("eta");
                iSubString.erase(varPosition, 3);
                eventFunctions.push_back( bind(&EventContainer::genjeteta, &eventContainer, getIndex(iSubString, iString)) );
                if( minFlag )
                    ComparisonTypes.push_back( ComparisonType::ABS_GREATER_THAN );
                else
                    ComparisonTypes.push_back( ComparisonType::ABS_SMALLER_THAN );
            }
            else if( iSubString.find("mass") != string::npos ) 
            {
                string::size_type varPosition = iSubString.find("mass");
                iSubString.erase(varPosition, 4);
                eventFunctions.push_back( bind(&EventContainer::genjetmass, &eventContainer, getIndex(iSubString, iString)) );
                if( maxFlag )
                    ComparisonTypes.push_back( ComparisonType::SMALLER_THAN );
                else
                    ComparisonTypes.push_back( ComparisonType::GREATER_THAN );
            }
            else if( iSubString.find("nhardprocess") != string::npos ) 
            {
                eventFunctions.push_back( bind(&EventContainer::genjetnhardprocess, &eventContainer) );
                if( maxFlag )
                    ComparisonTypes.push_back( ComparisonType::SMALLER_THAN );
                else if ( minFlag )
                    ComparisonTypes.push_back( ComparisonType::GREATER_THAN );
                else 
                    ComparisonTypes.push_back( ComparisonType::EQUAL );
            }
            else
            {
                cerr << "Variable '" << iString << "' in cuts or variables list not known." << endl;
                throw 1;
            }
        }
        else if( iSubString.find("puppijet") != string::npos )
        {
            string::size_type jetPosition = iSubString.find("puppijet");
            iSubString.erase(jetPosition, 7);

            if( iSubString.find("pt") != string::npos ) 
            {
                string::size_type varPosition = iSubString.find("pt");
                iSubString.erase(varPosition, 2);
                eventFunctions.push_back( bind(&EventContainer::puppijetpt, &eventContainer, getIndex(iSubString, iString)) );
                if( maxFlag )
                    ComparisonTypes.push_back( ComparisonType::SMALLER_THAN );
                else
                    ComparisonTypes.push_back( ComparisonType::GREATER_THAN );
            }
            else if( iSubString.find("eta") != string::npos ) 
            {
                string::size_type varPosition = iSubString.find("eta");
                iSubString.erase(varPosition, 3);
                eventFunctions.push_back( bind(&EventContainer::puppijeteta, &eventContainer, getIndex(iSubString, iString)) );
                if( minFlag )
                    ComparisonTypes.push_back( ComparisonType::ABS_GREATER_THAN );
                else
                    ComparisonTypes.push_back( ComparisonType::ABS_SMALLER_THAN );
            }
            else if( iSubString.find("mass") != string::npos ) 
            {
                string::size_type varPosition = iSubString.find("mass");
                iSubString.erase(varPosition, 4);
                eventFunctions.push_back( bind(&EventContainer::puppijetmass, &eventContainer, getIndex(iSubString, iString)) );
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
        else if( iSubString.find("trackjet") != string::npos )
        {
            string::size_type jetPosition = iSubString.find("trackjet");
            iSubString.erase(jetPosition, 8);

            if( iSubString.find("htdensity") != string::npos ) 
            {
                eventFunctions.push_back( bind(&EventContainer::trackjethtdensity, &eventContainer) );
                if( minFlag )
                    ComparisonTypes.push_back( ComparisonType::GREATER_THAN );
                else
                    ComparisonTypes.push_back( ComparisonType::SMALLER_THAN );
            }
            else if( iSubString.find("ndensity") != string::npos ) 
            {
                eventFunctions.push_back( bind(&EventContainer::trackjetndensity, &eventContainer) );
                if( minFlag )
                    ComparisonTypes.push_back( ComparisonType::GREATER_THAN );
                else
                    ComparisonTypes.push_back( ComparisonType::SMALLER_THAN );
            }
            else if( iSubString.find("ht") != string::npos ) 
            {
                eventFunctions.push_back( bind(&EventContainer::trackjetht, &eventContainer) );
                if( minFlag )
                    ComparisonTypes.push_back( ComparisonType::GREATER_THAN );
                else
                    ComparisonTypes.push_back( ComparisonType::SMALLER_THAN );
            }
            else if( iSubString.find("n") != string::npos && iSubString.size() < 2 ) 
            {
                eventFunctions.push_back( bind(&EventContainer::trackjetn, &eventContainer) );
                if( minFlag )
                    ComparisonTypes.push_back( ComparisonType::GREATER_THAN );
                else
                    ComparisonTypes.push_back( ComparisonType::SMALLER_THAN );
            }
            else
            {
                cerr << "Variable '" << iString << "' in cuts or variables list not known." << endl;
                throw 1;
            }
        }
        else if( iSubString.find("jet") != string::npos )
        {
            string::size_type jetPosition = iSubString.find("jet");
            iSubString.erase(jetPosition, 3);

            if( iSubString.find("isolatedbpt") != string::npos ) 
            {
                string::size_type varPosition = iSubString.find("isolatedbpt");
                iSubString.erase(varPosition, 11);
                eventFunctions.push_back( bind(&EventContainer::isolatedbjetpt, &eventContainer, getFloat(iSubString, iString)) );
                if( maxFlag )
                    ComparisonTypes.push_back( ComparisonType::SMALLER_THAN );
                else
                    ComparisonTypes.push_back( ComparisonType::GREATER_THAN );
            }
            else if( iSubString.find("isolatedpt") != string::npos ) 
            {
                eventFunctions.push_back( bind(&EventContainer::isolatedjetpt, &eventContainer) );
                if( maxFlag )
                    ComparisonTypes.push_back( ComparisonType::SMALLER_THAN );
                else
                    ComparisonTypes.push_back( ComparisonType::GREATER_THAN );
            }
            else if( iSubString.find("leptonclosest") != string::npos ) 
            {
                // jet closest to lepton variables
                string::size_type leptonPosition = iSubString.find("leptonclosest");
                iSubString.erase(leptonPosition, 13);
                
                if( iSubString.find("loosept") != string::npos ) 
                {
                    string::size_type varPosition = iSubString.find("loosept");
                    iSubString.erase(varPosition, 7);
                    eventFunctions.push_back( bind(&EventContainer::looseleptonclosestjetpt, &eventContainer, getIndex(iSubString, iString)) );
                    if( maxFlag )
                        ComparisonTypes.push_back( ComparisonType::SMALLER_THAN );
                    else
                        ComparisonTypes.push_back( ComparisonType::GREATER_THAN );
                }
                else if( iSubString.find("loosedr") != string::npos ) 
                {
                    string::size_type varPosition = iSubString.find("loosedr");
                    iSubString.erase(varPosition, 7);
                    eventFunctions.push_back( bind(&EventContainer::looseleptonclosestjetdr, &eventContainer, getIndex(iSubString, iString)) );
                    if( minFlag )
                        ComparisonTypes.push_back( ComparisonType::ABS_GREATER_THAN );
                    else
                        ComparisonTypes.push_back( ComparisonType::ABS_SMALLER_THAN );
                }
                else if( iSubString.find("looseflavour") != string::npos )
                {
                    string::size_type varPosition = iSubString.find("looseflavour");
                    iSubString.erase(varPosition, 12);
                    eventFunctions.push_back( bind(&EventContainer::looseleptonclosestjetpartonflavour, &eventContainer, getIndex(iSubString, iString)) );
                    if( maxFlag )
                        ComparisonTypes.push_back( ComparisonType::SMALLER_THAN );
                    else if( minFlag )
                        ComparisonTypes.push_back( ComparisonType::GREATER_THAN );
                    else
                        ComparisonTypes.push_back( ComparisonType::EQUAL );
                }
                else if( iSubString.find("pt") != string::npos ) 
                {
                    string::size_type varPosition = iSubString.find("pt");
                    iSubString.erase(varPosition, 2);
                    eventFunctions.push_back( bind(&EventContainer::leptonclosestjetpt, &eventContainer, getIndex(iSubString, iString)) );
                    if( maxFlag )
                        ComparisonTypes.push_back( ComparisonType::SMALLER_THAN );
                    else
                        ComparisonTypes.push_back( ComparisonType::GREATER_THAN );
                }
                else if( iSubString.find("dr") != string::npos ) 
                {
                    string::size_type varPosition = iSubString.find("dr");
                    iSubString.erase(varPosition, 2);
                    eventFunctions.push_back( bind(&EventContainer::leptonclosestjetdr, &eventContainer, getIndex(iSubString, iString)) );
                    if( minFlag )
                        ComparisonTypes.push_back( ComparisonType::ABS_GREATER_THAN );
                    else
                        ComparisonTypes.push_back( ComparisonType::ABS_SMALLER_THAN );
                }
                else if( iSubString.find("flavour") != string::npos )
                {
                    string::size_type varPosition = iSubString.find("flavour");
                    iSubString.erase(varPosition, 7);
                    eventFunctions.push_back( bind(&EventContainer::leptonclosestjetpartonflavour, &eventContainer, getIndex(iSubString, iString)) );
                    if( maxFlag )
                        ComparisonTypes.push_back( ComparisonType::SMALLER_THAN );
                    else if( minFlag )
                        ComparisonTypes.push_back( ComparisonType::GREATER_THAN );
                    else
                        ComparisonTypes.push_back( ComparisonType::EQUAL );
                }
                else
                {
                    cerr << "Variable '" << iString << "' in cuts or variables list not known." << endl;
                    throw 1;
                }
            }
            else if( iSubString.find("pt") != string::npos ) 
            {
                string::size_type varPosition = iSubString.find("pt");
                iSubString.erase(varPosition, 2);
                eventFunctions.push_back( bind(&EventContainer::jetpt, &eventContainer, getIndex(iSubString, iString)) );
                if( maxFlag )
                    ComparisonTypes.push_back( ComparisonType::SMALLER_THAN );
                else
                    ComparisonTypes.push_back( ComparisonType::GREATER_THAN );
            }
            else if( iSubString.find("eta") != string::npos ) 
            {
                string::size_type varPosition = iSubString.find("eta");
                iSubString.erase(varPosition, 3);
                eventFunctions.push_back( bind(&EventContainer::jeteta, &eventContainer, getIndex(iSubString, iString)) );
                if( minFlag )
                    ComparisonTypes.push_back( ComparisonType::ABS_GREATER_THAN );
                else
                    ComparisonTypes.push_back( ComparisonType::ABS_SMALLER_THAN );
            }
            else if( iSubString.find("mass") != string::npos ) 
            {
                string::size_type varPosition = iSubString.find("mass");
                iSubString.erase(varPosition, 4);
                eventFunctions.push_back( bind(&EventContainer::jetmass, &eventContainer, getIndex(iSubString, iString)) );
                if( maxFlag )
                    ComparisonTypes.push_back( ComparisonType::SMALLER_THAN );
                else
                    ComparisonTypes.push_back( ComparisonType::GREATER_THAN );
            }
            else if( iSubString.find("csv") != string::npos ) 
            {
                string::size_type varPosition = iSubString.find("csv");
                iSubString.erase(varPosition, 3);
                eventFunctions.push_back( bind(&EventContainer::jetcsv, &eventContainer, getIndex(iSubString, iString)) );
                if( maxFlag )
                    ComparisonTypes.push_back( ComparisonType::SMALLER_THAN );
                else
                    ComparisonTypes.push_back( ComparisonType::GREATER_THAN );
            }
            else if( iSubString.find("bveto") != string::npos ) 
            {
                string::size_type varPosition = iSubString.find("bveto");
                iSubString.erase(varPosition, 5);
                eventFunctions.push_back( bind(&EventContainer::jetmaxcsv, &eventContainer, getFloat(iSubString, iString)) );
                if( minFlag )
                    ComparisonTypes.push_back( ComparisonType::GREATER_THAN );
                else
                    ComparisonTypes.push_back( ComparisonType::SMALLER_THAN );
            }
            else if( iSubString.find("btag") != string::npos ) 
            {
                string::size_type varPosition = iSubString.find("btag");
                iSubString.erase(varPosition, 4);
                eventFunctions.push_back( bind(&EventContainer::jetmaxcsv, &eventContainer, getFloat(iSubString, iString)) );
                if( maxFlag )
                    ComparisonTypes.push_back( ComparisonType::SMALLER_THAN );
                else
                    ComparisonTypes.push_back( ComparisonType::GREATER_THAN );
            }
            else if( iSubString.find("qglikelihood") != string::npos ) 
            {
                string::size_type varPosition = iSubString.find("qglikelihood");
                iSubString.erase(varPosition, 12);
                eventFunctions.push_back( bind(&EventContainer::jetqglikelihood, &eventContainer, getIndex(iSubString, iString)) );
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
        else if( iSubString.find("genlepton") != string::npos )
        {
            string::size_type leptonPosition = iSubString.find("genlepton");
            iSubString.erase(leptonPosition, 9);
            
            if( iSubString.find("pt") != string::npos ) 
            {
                string::size_type varPosition = iSubString.find("pt");
                iSubString.erase(varPosition, 2);
                eventFunctions.push_back( bind(&EventContainer::genleptonpt, &eventContainer, getIndex(iSubString, iString)) );
                if( maxFlag )
                    ComparisonTypes.push_back( ComparisonType::SMALLER_THAN );
                else
                    ComparisonTypes.push_back( ComparisonType::GREATER_THAN );
            }
            else if( iSubString.find("eta") != string::npos ) 
            {
                string::size_type varPosition = iSubString.find("eta");
                iSubString.erase(varPosition, 3);
                eventFunctions.push_back( bind(&EventContainer::genleptoneta, &eventContainer, getIndex(iSubString, iString)) );
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
        else if( iSubString.find("nlooseleptons") != string::npos ) 
        {
            string::size_type varPosition = iSubString.find("nlooseleptons");
            iSubString.erase(varPosition, 13);
            eventFunctions.push_back( bind(&EventContainer::nlooseleptons, &eventContainer, getFloat(iSubString, iString)) );
            if( maxFlag )
                ComparisonTypes.push_back( ComparisonType::SMALLER_THAN );
            else
                ComparisonTypes.push_back( ComparisonType::GREATER_THAN );
        }
        else if( iSubString.find("looselepton") != string::npos )
        {
            string::size_type leptonPosition = iSubString.find("looselepton");
            iSubString.erase(leptonPosition, 11);
            
            if( iSubString.find("correctedpt") != string::npos ) 
            {
                string::size_type varPosition = iSubString.find("correctedpt");
                iSubString.erase(varPosition, 11);
                eventFunctions.push_back( bind(&EventContainer::looseleptoncorrectedpt, &eventContainer, getIndex(iSubString, iString)) );
                if( maxFlag )
                    ComparisonTypes.push_back( ComparisonType::SMALLER_THAN );
                else
                    ComparisonTypes.push_back( ComparisonType::GREATER_THAN );
            }
            else if( iSubString.find("pt") != string::npos ) 
            {
                string::size_type varPosition = iSubString.find("pt");
                iSubString.erase(varPosition, 2);
                eventFunctions.push_back( bind(&EventContainer::looseleptonpt, &eventContainer, getIndex(iSubString, iString)) );
                if( maxFlag )
                    ComparisonTypes.push_back( ComparisonType::SMALLER_THAN );
                else
                    ComparisonTypes.push_back( ComparisonType::GREATER_THAN );
            }
            else if( iSubString.find("abseta") != string::npos )
            {
                string::size_type varPosition = iSubString.find("abseta");
                iSubString.erase(varPosition, 6);
                eventFunctions.push_back( bind(&EventContainer::looseleptonabseta, &eventContainer, getIndex(iSubString, iString)) );
                if( minFlag )
                    ComparisonTypes.push_back( ComparisonType::GREATER_THAN );
                else
                    ComparisonTypes.push_back( ComparisonType::SMALLER_THAN );
            }
            else if( iSubString.find("eta") != string::npos ) 
            {
                string::size_type varPosition = iSubString.find("eta");
                iSubString.erase(varPosition, 3);
                eventFunctions.push_back( bind(&EventContainer::looseleptoneta, &eventContainer, getIndex(iSubString, iString)) );
                if( minFlag )
                    ComparisonTypes.push_back( ComparisonType::ABS_GREATER_THAN );
                else
                    ComparisonTypes.push_back( ComparisonType::ABS_SMALLER_THAN );
            }
            else if( iSubString.find("flavour") != string::npos ) 
            {
                string::size_type varPosition = iSubString.find("flavour");
                iSubString.erase(varPosition, 7);
                eventFunctions.push_back( bind(&EventContainer::looseleptonflavour, &eventContainer, getIndex(iSubString, iString)) );
                ComparisonTypes.push_back( ComparisonType::EQUAL );
            }
            else if( iSubString.find("isolation") != string::npos ) 
            {
                string::size_type varPosition = iSubString.find("isolation");
                iSubString.erase(varPosition, 9);
                eventFunctions.push_back( bind(&EventContainer::looseleptonisolation, &eventContainer, getIndex(iSubString, iString)) );
                if( minFlag )
                    ComparisonTypes.push_back( ComparisonType::GREATER_THAN );
                else
                    ComparisonTypes.push_back( ComparisonType::SMALLER_THAN );
            }
            else if( iSubString.find("passtight") != string::npos ) 
            {
                string::size_type varPosition = iSubString.find("passtight");
                iSubString.erase(varPosition, 9);
                eventFunctions.push_back( bind(&EventContainer::looseleptonpasstight, &eventContainer, getIndex(iSubString, iString)) );
                ComparisonTypes.push_back( ComparisonType::EQUAL );
            }
            else
            {
                cerr << "Variable '" << iString << "' in cuts or variables list not known." << endl;
                throw 1;
            }
        }
        
        
        else if( iSubString.find("nleptons") != string::npos ) 
        {
            string::size_type varPosition = iSubString.find("nleptons");
            iSubString.erase(varPosition, 8);
            eventFunctions.push_back( bind(&EventContainer::nLeptons, &eventContainer, getFloat(iSubString, iString)) );
            if( maxFlag )
                ComparisonTypes.push_back( ComparisonType::SMALLER_THAN );
            else
                ComparisonTypes.push_back( ComparisonType::GREATER_THAN );
        }
        else if( iSubString.find("lepton") != string::npos )
        {
            string::size_type leptonPosition = iSubString.find("lepton");
            iSubString.erase(leptonPosition, 6);
            
            if( iSubString.find("looseflavourpt") != string::npos ) 
            {
                string::size_type varPosition = iSubString.find("looseflavourpt");
                iSubString.erase(varPosition, 14);
                eventFunctions.push_back( bind(&EventContainer::looseflavourleptonpt, &eventContainer, getFloat(iSubString, iString)) );
                if( maxFlag )
                    ComparisonTypes.push_back( ComparisonType::SMALLER_THAN );
                else
                    ComparisonTypes.push_back( ComparisonType::GREATER_THAN );
            }
            else if( iSubString.find("flavourpt") != string::npos ) 
            {
                string::size_type varPosition = iSubString.find("flavourpt");
                iSubString.erase(varPosition, 9);
                eventFunctions.push_back( bind(&EventContainer::flavourleptonpt, &eventContainer, getFloat(iSubString, iString)) );
                if( maxFlag )
                    ComparisonTypes.push_back( ComparisonType::SMALLER_THAN );
                else
                    ComparisonTypes.push_back( ComparisonType::GREATER_THAN );
            }
            else if( iSubString.find("correctedpt") != string::npos ) 
            {
                string::size_type varPosition = iSubString.find("correctedpt");
                iSubString.erase(varPosition, 11);
                eventFunctions.push_back( bind(&EventContainer::leptoncorrectedpt, &eventContainer, getIndex(iSubString, iString)) );
                if( maxFlag )
                    ComparisonTypes.push_back( ComparisonType::SMALLER_THAN );
                else
                    ComparisonTypes.push_back( ComparisonType::GREATER_THAN );
            }
            else if( iSubString.find("pt") != string::npos ) 
            {
                string::size_type varPosition = iSubString.find("pt");
                iSubString.erase(varPosition, 2);
                eventFunctions.push_back( bind(&EventContainer::leptonpt, &eventContainer, getIndex(iSubString, iString)) );
                if( maxFlag )
                    ComparisonTypes.push_back( ComparisonType::SMALLER_THAN );
                else
                    ComparisonTypes.push_back( ComparisonType::GREATER_THAN );
            }
            else if( iSubString.find("abseta") != string::npos )
            {
                string::size_type varPosition = iSubString.find("abseta");
                iSubString.erase(varPosition, 6);
                eventFunctions.push_back( bind(&EventContainer::leptonabseta, &eventContainer, getIndex(iSubString, iString)) );
                if( minFlag )
                    ComparisonTypes.push_back( ComparisonType::GREATER_THAN );
                else
                    ComparisonTypes.push_back( ComparisonType::SMALLER_THAN );
            }
            else if( iSubString.find("eta") != string::npos ) 
            {
                string::size_type varPosition = iSubString.find("eta");
                iSubString.erase(varPosition, 3);
                eventFunctions.push_back( bind(&EventContainer::leptoneta, &eventContainer, getIndex(iSubString, iString)) );
                if( minFlag )
                    ComparisonTypes.push_back( ComparisonType::ABS_GREATER_THAN );
                else
                    ComparisonTypes.push_back( ComparisonType::ABS_SMALLER_THAN );
            }
            else if( iSubString == "looseproductcharge" ) 
            {
                eventFunctions.push_back( bind(&EventContainer::looseproductleptoncharge, &eventContainer) );
                if( maxFlag )
                    ComparisonTypes.push_back( ComparisonType::SMALLER_THAN );
                else if( minFlag )
                    ComparisonTypes.push_back( ComparisonType::GREATER_THAN );
                else
                    ComparisonTypes.push_back( ComparisonType::EQUAL );
            }
            else if( iSubString == "productcharge" ) 
            {
                eventFunctions.push_back( bind(&EventContainer::productleptoncharge, &eventContainer) );
                if( maxFlag )
                    ComparisonTypes.push_back( ComparisonType::SMALLER_THAN );
                else if( minFlag )
                    ComparisonTypes.push_back( ComparisonType::GREATER_THAN );
                else
                    ComparisonTypes.push_back( ComparisonType::EQUAL );
            }
            else if( iSubString.find("isolation") != string::npos ) 
            {
                string::size_type varPosition = iSubString.find("isolation");
                iSubString.erase(varPosition, 9);
                eventFunctions.push_back( bind(&EventContainer::leptonisolation, &eventContainer, getIndex(iSubString, iString)) );
                if( minFlag )
                    ComparisonTypes.push_back( ComparisonType::GREATER_THAN );
                else
                    ComparisonTypes.push_back( ComparisonType::SMALLER_THAN );
            }
            else if( iSubString.find("triplechargeagreement") != string::npos ) 
            {
                string::size_type varPosition = iSubString.find("triplechargeagreement");
                iSubString.erase(varPosition, 21);
                eventFunctions.push_back( bind(&EventContainer::leptontriplechargeagreement, &eventContainer, getIndex(iSubString, iString)) );
                ComparisonTypes.push_back( ComparisonType::EQUAL );
            }
            else if( iSubString.find("charge") != string::npos ) 
            {
                string::size_type varPosition = iSubString.find("charge");
                iSubString.erase(varPosition, 6);
                eventFunctions.push_back( bind(&EventContainer::leptoncharge, &eventContainer, getIndex(iSubString, iString)) );
                if( maxFlag )
                    ComparisonTypes.push_back( ComparisonType::SMALLER_THAN );
                else if( minFlag )
                    ComparisonTypes.push_back( ComparisonType::GREATER_THAN );
                else
                    ComparisonTypes.push_back( ComparisonType::EQUAL );
            }
            else
            {
                cerr << "Variable '" << iString << "' in cuts or variables list not known." << endl;
                throw 1;
            }
        }   
    
        else if( iSubString.find("electron") != string::npos )
        {
            string::size_type leptonPosition = iSubString.find("electron");
            iSubString.erase(leptonPosition, 8);
            
            if( iSubString.find("d0") != string::npos ) 
            {
                string::size_type varPosition = iSubString.find("d0");
                iSubString.erase(varPosition, 2);
                eventFunctions.push_back( bind(&EventContainer::electrond0, &eventContainer, getIndex(iSubString, iString)) );
                if( minFlag )
                    ComparisonTypes.push_back( ComparisonType::ABS_GREATER_THAN );
                else
                    ComparisonTypes.push_back( ComparisonType::ABS_SMALLER_THAN );
            }
            else if( iSubString.find("detain") != string::npos ) 
            {
                string::size_type varPosition = iSubString.find("detain");
                iSubString.erase(varPosition, 6);
                eventFunctions.push_back( bind(&EventContainer::electrondEtaIn, &eventContainer, getIndex(iSubString, iString)) );
                if( minFlag )
                    ComparisonTypes.push_back( ComparisonType::ABS_GREATER_THAN );
                else
                    ComparisonTypes.push_back( ComparisonType::ABS_SMALLER_THAN );
            }
            else if( iSubString.find("dphiin") != string::npos ) 
            {
                string::size_type varPosition = iSubString.find("dphiin");
                iSubString.erase(varPosition, 6);
                eventFunctions.push_back( bind(&EventContainer::electrondPhiIn, &eventContainer, getIndex(iSubString, iString)) );
                if( minFlag )
                    ComparisonTypes.push_back( ComparisonType::ABS_GREATER_THAN );
                else
                    ComparisonTypes.push_back( ComparisonType::ABS_SMALLER_THAN );
            }
            else if( iSubString.find("dz") != string::npos ) 
            {
                string::size_type varPosition = iSubString.find("dz");
                iSubString.erase(varPosition, 2);
                eventFunctions.push_back( bind(&EventContainer::electrondz, &eventContainer, getIndex(iSubString, iString)) );
                if( minFlag )
                    ComparisonTypes.push_back( ComparisonType::ABS_GREATER_THAN );
                else
                    ComparisonTypes.push_back( ComparisonType::ABS_SMALLER_THAN );
            }
            else if( iSubString.find("effarea") != string::npos ) 
            {
                string::size_type varPosition = iSubString.find("effarea");
                iSubString.erase(varPosition, 7);
                eventFunctions.push_back( bind(&EventContainer::electroneffectiveArea, &eventContainer, getIndex(iSubString, iString)) );
                if( minFlag )
                    ComparisonTypes.push_back( ComparisonType::GREATER_THAN );
                else
                    ComparisonTypes.push_back( ComparisonType::SMALLER_THAN );
            }
            else if( iSubString.find("misshits") != string::npos ) 
            {
                string::size_type varPosition = iSubString.find("misshits");
                iSubString.erase(varPosition, 8);
                eventFunctions.push_back( bind(&EventContainer::electronmissingHits, &eventContainer, getIndex(iSubString, iString)) );
                if( minFlag )
                    ComparisonTypes.push_back( ComparisonType::GREATER_THAN );
                else
                    ComparisonTypes.push_back( ComparisonType::SMALLER_THAN );
            }
            else if( iSubString.find("sietaieta") != string::npos ) 
            {
                string::size_type varPosition = iSubString.find("sietaieta");
                iSubString.erase(varPosition, 9);
                eventFunctions.push_back( bind(&EventContainer::electronsigmaIetaIeta, &eventContainer, getIndex(iSubString, iString)) );
                if( minFlag )
                    ComparisonTypes.push_back( ComparisonType::GREATER_THAN );
                else
                    ComparisonTypes.push_back( ComparisonType::SMALLER_THAN );
            }
            else if( iSubString.find("hovere") != string::npos ) 
            {
                string::size_type varPosition = iSubString.find("hovere");
                iSubString.erase(varPosition, 6);
                eventFunctions.push_back( bind(&EventContainer::electronhOverE, &eventContainer, getIndex(iSubString, iString)) );
                if( minFlag )
                    ComparisonTypes.push_back( ComparisonType::GREATER_THAN );
                else
                    ComparisonTypes.push_back( ComparisonType::SMALLER_THAN );
            }
            else if( iSubString.find("ooemoop") != string::npos ) 
            {
                string::size_type varPosition = iSubString.find("ooemoop");
                iSubString.erase(varPosition, 7);
                eventFunctions.push_back( bind(&EventContainer::electronooEmoop, &eventContainer, getIndex(iSubString, iString)) );
                if( minFlag )
                    ComparisonTypes.push_back( ComparisonType::GREATER_THAN );
                else
                    ComparisonTypes.push_back( ComparisonType::SMALLER_THAN );
            }
            else if( iSubString.find("convveto") != string::npos ) 
            {
                string::size_type varPosition = iSubString.find("convveto");
                iSubString.erase(varPosition, 8);
                eventFunctions.push_back( bind(&EventContainer::electronconversionVeto, &eventContainer, getIndex(iSubString, iString)) );
                ComparisonTypes.push_back( ComparisonType::EQUAL );
            }
            else if( iSubString.find("sceta") != string::npos ) 
            {
                string::size_type varPosition = iSubString.find("sceta");
                iSubString.erase(varPosition, 5);
                eventFunctions.push_back( bind(&EventContainer::electronscEta, &eventContainer, getIndex(iSubString, iString)) );
                if( minFlag )
                    ComparisonTypes.push_back( ComparisonType::ABS_GREATER_THAN );
                else
                    ComparisonTypes.push_back( ComparisonType::ABS_SMALLER_THAN );
            }
            else if( iSubString.find("isolation") != string::npos ) 
            {
                string::size_type varPosition = iSubString.find("isolation");
                iSubString.erase(varPosition, 9);
                eventFunctions.push_back( bind(&EventContainer::electronisolation, &eventContainer, getIndex(iSubString, iString)) );
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
        
        // MET
        else if( iSubString == "met" ) 
        {
            eventFunctions.push_back( bind(&Met::pt, &(eventContainer.met)) );
            if( maxFlag )
                ComparisonTypes.push_back( ComparisonType::SMALLER_THAN );
            else
                ComparisonTypes.push_back( ComparisonType::GREATER_THAN );
        }
        
        // PU
        else if( iSubString == "nvertices" ) 
        {
            eventFunctions.push_back( bind(&EventContainer::nvertices, &eventContainer) );
            if( maxFlag )
                ComparisonTypes.push_back( ComparisonType::SMALLER_THAN );
            else
                ComparisonTypes.push_back( ComparisonType::GREATER_THAN );
        }
        
        // Diparticle variables
        else if( iSubString == "genmll" ) 
        {
            eventFunctions.push_back( bind(&EventContainer::genmll, &eventContainer) );
            if( maxFlag )
                ComparisonTypes.push_back( ComparisonType::SMALLER_THAN );
            else
                ComparisonTypes.push_back( ComparisonType::GREATER_THAN );
        }
        else if( iSubString == "loosemll" ) 
        {
            eventFunctions.push_back( bind(&EventContainer::loosemll, &eventContainer) );
            if( maxFlag )
                ComparisonTypes.push_back( ComparisonType::SMALLER_THAN );
            else
                ComparisonTypes.push_back( ComparisonType::GREATER_THAN );
        }
        else if( iSubString == "mll" ) 
        {
            eventFunctions.push_back( bind(&EventContainer::mll, &eventContainer) );
            if( maxFlag )
                ComparisonTypes.push_back( ComparisonType::SMALLER_THAN );
            else
                ComparisonTypes.push_back( ComparisonType::GREATER_THAN );
        }
        else if( iSubString.find("zveto") != string::npos ) 
        {
            string::size_type varPosition = iSubString.find("zveto");
            iSubString.erase(varPosition, 5);
            
            if( iSubString.size() > 0 ) 
                eventFunctions.push_back( bind(&EventContainer::dmllminpt, &eventContainer, 91, getFloat(iSubString, iString)) );
            else
                eventFunctions.push_back( bind(&EventContainer::dmll, &eventContainer, 91) );
            ComparisonTypes.push_back( ComparisonType::ABS_GREATER_THAN );
        }
        else if( iSubString.find("zsel") != string::npos ) 
        {
            string::size_type varPosition = iSubString.find("zsel");
            iSubString.erase(varPosition, 4);
            
            if( iSubString.size() > 0 ) 
                eventFunctions.push_back( bind(&EventContainer::dmllminpt, &eventContainer, 91, getFloat(iSubString, iString)) );
            else
                eventFunctions.push_back( bind(&EventContainer::dmll, &eventContainer, 91) );
            ComparisonTypes.push_back( ComparisonType::ABS_SMALLER_THAN );
        }
        else if( iSubString == "zeeveto"  ) 
        {
            eventFunctions.push_back( bind(&EventContainer::dmee, &eventContainer, 91) );
            ComparisonTypes.push_back( ComparisonType::ABS_GREATER_THAN );
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
        else if( iSubString == "drll" ) 
        {
            eventFunctions.push_back( bind(&EventContainer::drll, &eventContainer) );
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
        else if( iSubString == "dphilj" ) 
        {
            eventFunctions.push_back( bind(&EventContainer::dphilj, &eventContainer) );
            if( maxFlag )
                ComparisonTypes.push_back( ComparisonType::ABS_SMALLER_THAN );
            else
                ComparisonTypes.push_back( ComparisonType::ABS_GREATER_THAN ); 
        }
        else if( iSubString == "ptloverj" ) 
        {
            eventFunctions.push_back( bind(&EventContainer::ptloverj, &eventContainer) );
            if( maxFlag )
                ComparisonTypes.push_back( ComparisonType::SMALLER_THAN );
            else
                ComparisonTypes.push_back( ComparisonType::GREATER_THAN ); 
        }
        
        // Special variables
        else if( iSubString == "genchannel" ) 
        {
            eventFunctions.push_back( bind(&EventContainer::genchannel, &eventContainer) );
            ComparisonTypes.push_back( ComparisonType::EQUAL );
        }
        else if( iSubString == "loosechannel" ) 
        {
            eventFunctions.push_back( bind(&EventContainer::loosechannel, &eventContainer) );
            if( maxFlag )
                ComparisonTypes.push_back( ComparisonType::SMALLER_THAN );
            else if( minFlag )
                ComparisonTypes.push_back( ComparisonType::GREATER_THAN );
            else
                ComparisonTypes.push_back( ComparisonType::EQUAL );
        }
        else if( iSubString == "channel" ) 
        {
            eventFunctions.push_back( bind(&EventContainer::channel, &eventContainer) );
            if( maxFlag )
                ComparisonTypes.push_back( ComparisonType::SMALLER_THAN );
            else if( minFlag )
                ComparisonTypes.push_back( ComparisonType::GREATER_THAN );
            else
                ComparisonTypes.push_back( ComparisonType::EQUAL );
        }
        else if( iSubString == "drjjll" ) 
        {
            eventFunctions.push_back( bind(&EventContainer::drjjll, &eventContainer) );
            if( minFlag )
                ComparisonTypes.push_back( ComparisonType::GREATER_THAN );
            else
                ComparisonTypes.push_back( ComparisonType::SMALLER_THAN );
        }        
        else if( iSubString.find("zeppenfeldl") != string::npos ) 
        {
            string::size_type varPosition = iSubString.find("zeppenfeldl");
            iSubString.erase(varPosition, 11);
            eventFunctions.push_back( bind(&EventContainer::zeppenfeldlep, &eventContainer, getIndex(iSubString, iString)) );
            if( minFlag )
                ComparisonTypes.push_back( ComparisonType::ABS_GREATER_THAN );
            else
                ComparisonTypes.push_back( ComparisonType::ABS_SMALLER_THAN );
        }
        else if( iSubString.find("mt") != string::npos ) 
        {
            eventFunctions.push_back( bind(&EventContainer::mt, &eventContainer) );
            if( maxFlag )
                ComparisonTypes.push_back( ComparisonType::SMALLER_THAN );
            else
                ComparisonTypes.push_back( ComparisonType::GREATER_THAN );
        }
        else if( iSubString.find("passzfr") != string::npos ) 
        {
            string::size_type varPosition = iSubString.find("passzfr");
            iSubString.erase(varPosition, 7);
            eventFunctions.push_back( bind(&EventContainer::passZjetFR, &eventContainer, getFloat(iSubString, iString)) );
            ComparisonTypes.push_back( ComparisonType::EQUAL );
        }
        else if( iSubString == "eventno" ) 
        {
            eventFunctions.push_back( bind(&EventContainer::eventNo, &eventContainer) );
            if( maxFlag )
                ComparisonTypes.push_back( ComparisonType::SMALLER_THAN );
            else if( minFlag )
                ComparisonTypes.push_back( ComparisonType::GREATER_THAN );
            else
                ComparisonTypes.push_back( ComparisonType::EQUAL );
        }
        else if( iSubString.find("softmuonveto") != string::npos ) 
        {
            string::size_type varPosition = iSubString.find("softmuonveto");
            iSubString.erase(varPosition, 12);
            eventFunctions.push_back( bind(&EventContainer::jetmaxSoftMuonPt, &eventContainer, getFloat(iSubString, iString)) );
            ComparisonTypes.push_back( ComparisonType::SMALLER_THAN );
        }
        else if( iSubString == "detametl" ) 
        {
            eventFunctions.push_back( bind(&EventContainer::detametl, &eventContainer) );
            if( maxFlag )
                ComparisonTypes.push_back( ComparisonType::ABS_SMALLER_THAN );
            else
                ComparisonTypes.push_back( ComparisonType::ABS_GREATER_THAN ); 
        }
        else if( iSubString == "mmetl" ) 
        {
            eventFunctions.push_back( bind(&EventContainer::mmetl, &eventContainer) );
            if( maxFlag )
                ComparisonTypes.push_back( ComparisonType::SMALLER_THAN );
            else
                ComparisonTypes.push_back( ComparisonType::GREATER_THAN ); 
        }
        
        // new Vetos
        else if( iSubString == "passZrecoVeto" ) 
        {
            eventFunctions.push_back( bind(&EventContainer::passZrecoVeto, &eventContainer) );
            ComparisonTypes.push_back( ComparisonType::EQUAL );
        }
        else if( iSubString == "passZrecoVetoMuon" ) 
        {
            eventFunctions.push_back( bind(&EventContainer::passZrecoVetoMuon, &eventContainer) );
            ComparisonTypes.push_back( ComparisonType::EQUAL );
        }
        else if( iSubString == "tauveto" ) 
        {
            eventFunctions.push_back( bind(&EventContainer::passTauVeto, &eventContainer) );
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

unsigned int VariableDictionary::getIndex(const string& indexString, const string& fullString) const
{
    float value = getFloat(indexString, fullString);
    
    if( floor(value) != ceil(value) )
    {
        cerr << "Index is not an integer: " << fullString << endl;
        throw 1;
    }
    
    unsigned int index = (unsigned int) value;
    if( index == (unsigned) 0 )
    {
        cerr << "Index 0 for variable: " << fullString << endl;
        throw 1;        
    }
    
    // jet1 = jet with index 0
    index--;
    return index;    
}

float VariableDictionary::getFloat(const string& indexString, const string& fullString) const
{
    if( indexString.size() < 1 )
    {
        cerr << "Missing value for variable: " << fullString << endl;
        throw 1;
    }
    
    if( regex_search(indexString, regex("[A-Za-z]")) )
    {
        cerr << "float still contains letters, probably wrong dictionary match for: " << fullString << endl;
        throw 1;
    }
    
    float value = 0;
    try
    {
        value = stof(indexString);
    }
    catch(...)
    {
        cerr << "Retrieving value failed for \"" << indexString << "\" from variable: " << fullString << endl;
        throw;
    }
    
    return value;
    
}