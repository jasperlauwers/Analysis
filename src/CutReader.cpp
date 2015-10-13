
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
    
    for( unsigned int iCut=0; iCut < nCut; ++iCut ) 
    {
        int remainder = iCut%2;
        if( remainder == 0 ) 
        {
            string cutVariable = (cuts[iCut]).c_str();
            transform(cutVariable.begin(), cutVariable.end(), cutVariable.begin(), ::tolower); // convert to lower case
            cutContainer.variableNames.push_back( cutVariable );
        }
        else 
        {
            cutContainer.cutValues.push_back( cuts[iCut] );
        }
    }
}

CutReader::~CutReader() { }