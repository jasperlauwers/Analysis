
#include "CutReader.hpp"

CutReader::CutReader(const string& fileName, CutContainer& cutContainer)
: ConfigReader(fileName), cutContainer(cutContainer)
{
    // Allow float -> int and int -> float conversions
    cfg.setAutoConvert(true);
    
    const Setting& cuts = cfg.lookup("cuts");
    const unsigned int nCut = cuts.getLength();
    
    // Check # arguments
    if((nCut%2) != 0 ) 
    {
        cerr << "Number of elements in cuts list not correct." << endl;
        throw 1;
    }
    
    bool skipForTrigger = false;
    for( unsigned int iCut=0; iCut < nCut; ++iCut ) 
    {
        int remainder = iCut%2;
        if( remainder == 0 ) 
        {
            string cutVariable = (cuts[iCut]).c_str();
            transform(cutVariable.begin(), cutVariable.end(), cutVariable.begin(), ::tolower); // convert to lower case
            if( cutVariable == "trigger" )
            {
                const Setting& triggerSetting =  cuts[iCut+1];
                const Setting& indexSetting =  triggerSetting[0];
                const Setting& ptSetting =  triggerSetting[1];
                
                const unsigned int nTriggers = indexSetting.getLength();
                for( unsigned int iBin=0; iBin < nTriggers; ++iBin ) 
                {
                    cutContainer.triggerVector.push_back( indexSetting[iBin] );  
                    cutContainer.triggerPtVector.push_back( ptSetting[iBin] );  
                }
                
                if( triggerSetting.getLength() > 2 )
                {
                    const Setting& lumiSetting =  triggerSetting[2];
                    for( unsigned int iBin=0; iBin < nTriggers; ++iBin ) 
                    {
                        cutContainer.triggerLumiVector.push_back( lumiSetting[iBin] );  
                    }
                }
                
                if( nTriggers != (unsigned) ptSetting.getLength() || (triggerSetting.getLength() > 2  && nTriggers != (unsigned) triggerSetting[2].getLength() ) ) 
                {
                    cerr << "Number of elements in trigger cut list is not consistent." << endl;
                    throw 1;
                }
                
                skipForTrigger = true;
            }
            else
            {
                cutContainer.variableNames.push_back( cutVariable );
                skipForTrigger = false;
            }
        }
        else 
        {
            if( ! skipForTrigger )
                cutContainer.cutValues.push_back( cuts[iCut] );
        }
    }
}

CutReader::~CutReader() { }