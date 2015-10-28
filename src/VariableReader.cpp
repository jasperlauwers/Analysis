
#include "VariableReader.hpp"

VariableReader::VariableReader(const string& fileName, VariableContainer& varContainer)
: ConfigReader(fileName), variableContainer(varContainer)
{
    // Allow float -> int and int -> float conversions
    cfg.setAutoConvert(true);
    
    const Setting& variables = cfg.lookup("variables");
    const unsigned int nVar = variables.getLength();
    
    // Check # arguments
    if( (nVar%3) != 0 ) 
    {
        cerr << "Number of elements in variables list not correct." << endl;
        throw 1;
    }
    
    for( unsigned int iVar=0; iVar < nVar; ++iVar ) 
    {
        int remainder = iVar%3;
        if( remainder == 0 ) 
        {
            string varName = (variables[iVar]).c_str();
            transform(varName.begin(), varName.end(), varName.begin(), ::tolower); // convert to lower case
            variableContainer.variableNames.push_back( varName );
        }
        else if( remainder == 1 ) 
        {
            variableContainer.nBins.push_back( variables[iVar] );
        }
        else
        {
            const Setting& binSetting = variables[iVar];
            const unsigned int binLength = binSetting.getLength();
            if( binLength < 2 )
            {
                cerr << "Not enough numbers to make binning for variable: " << (variables[iVar-2]).c_str() << endl;
                throw 1;
            }
            
            vector<float> binVector(binLength);
            for( unsigned int iBin=0; iBin < binLength; ++iBin ) 
            {
                binVector[iBin] = (float) binSetting[iBin];  
            }
            variableContainer.binning.push_back( binVector );
        }
    }
}

VariableReader::~VariableReader() { }