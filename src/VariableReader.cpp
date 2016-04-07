
#include "VariableReader.hpp"

VariableReader::VariableReader(const string& fileName, VariableContainer& varContainer)
: ConfigReader(fileName), variableContainer(varContainer), is2D(false)
{
    // Allow float -> int and int -> float conversions
    cfg.setAutoConvert(true);
    
    const Setting& variables = cfg.lookup("variables");
    const unsigned int nVar = variables.getLength();
    
    unsigned int nRange = 0;
    for( unsigned int iVar=0; iVar < nVar; ++iVar ) 
    {
        int remainder = (iVar-nRange)%3;
        if( remainder == 0 ) 
        {
            const Setting& setting = variables[iVar];
            if( setting.getType() == Setting::Type::TypeString )
            {
                readVariableName(variables[iVar]);
            }
            else
            {
                const unsigned int axisLength = setting.getLength();
                if( axisLength != 2 )
                {
                    cerr << "Need exactly two numbers for y-axis range: " << (variables[iVar-3]).c_str() << endl;
                    throw 1;
                }
                vector<double> axisVector = {(double) setting[0], (double) setting[1]};  
                variableContainer.axisRanges[variableContainer.variableNames.size()-1] = ( axisVector );
                nRange++;
            }
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
            
            vector<double> binVector(binLength);
            for( unsigned int iBin=0; iBin < binLength; ++iBin ) 
            {
                binVector[iBin] = (double) binSetting[iBin];  
            }
            variableContainer.binning.push_back( binVector );
        }
    }
    
    // Check # arguments
    if( (nVar-variableContainer.axisRanges.size())%3 != 0 ) 
    {
        cerr << "Number of elements in variables list not correct." << endl;
        throw 1;
    }
}

VariableReader::~VariableReader() { }

void VariableReader::readVariableName(const Setting& varSetting)
{
    string varName = (varSetting).c_str();
    transform(varName.begin(), varName.end(), varName.begin(), ::tolower); // convert to lower case
    
    if( varName.find(":") != string::npos )
    {
        if( is2D )
            is2D = false;
        else
        {
            string::size_type splitPosition = varName.find(":");
            variableContainer.variableNames.push_back( varName.substr(0,splitPosition) );
            variableContainer.variableNames.push_back( varName.substr(splitPosition+1) );
            variableContainer.is2D.push_back(true);
            variableContainer.is2D.push_back(true);
            is2D = true;
        }
    }
    else 
    {
        variableContainer.variableNames.push_back( varName );
        variableContainer.is2D.push_back(false);
    }    
}