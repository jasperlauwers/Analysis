
#include "SampleReader.hpp"

SampleReader::SampleReader(const string& fileName, SampleContainer& sampleContainer)
: ConfigReader(fileName), sampleContainer(sampleContainer)
{
    // Allow float -> int and int -> float conversions
    cfg.setAutoConvert(true);
    
    const Setting& samples = cfg.lookup("samples");
    const unsigned int nSamples = samples.getLength();
    
    // Check # arguments
    if( (nSamples%6) != 0 ) 
    {
        cerr << "Number of elements in sample list not correct." << endl;
        throw 1;
    }
    
    unsigned int iSuperSample = -1, valuesPerLine = 6;
    bool newSuperSample = true;
    string previousReducedName = "", previousType;
    for( unsigned int iSample=0; iSample < nSamples; ++iSample ) 
    {
        int remainder = iSample%valuesPerLine;
        if( remainder == 0 ) 
        {
            string reducedName = (samples[iSample+1]).c_str(); // iSample+1 !
            
            if( reducedName != previousReducedName )
            {
                newSuperSample = true;
                iSuperSample++;
                previousReducedName = reducedName;
                sampleContainer.reducedNames.push_back( reducedName );
            }
            else
                newSuperSample = false;
        }
        else if( remainder == 1 ) 
        {
            string sampleName = (samples[iSample-1]).c_str(); // iSample-1 !
            
            if( newSuperSample ) 
            {
                vector<string> temp = { sampleName };
                sampleContainer.sampleNames.push_back( temp );
            }
            else
                sampleContainer.sampleNames[iSuperSample].push_back( sampleName );
        }
        else if( remainder == 2 ) 
        {
            if( newSuperSample ) 
            {
                vector<double> temp = { samples[iSample] };
                sampleContainer.luminosity.push_back( temp );
            }
            else
                sampleContainer.luminosity[iSuperSample].push_back( samples[iSample] );
        }
        else if( remainder == 3 ) 
        {
            if( newSuperSample ) 
            {
                vector<double> temp = { samples[iSample] };
                sampleContainer.weight.push_back( temp );
            }
            else
                sampleContainer.weight[iSuperSample].push_back( samples[iSample] );
        }
        else if( remainder == 4 ) 
        {
            string type = (samples[iSample]).c_str();
            transform(type.begin(), type.end(), type.begin(), ::tolower); // convert to lower case
            
            if( newSuperSample )
            {
                previousType = type;
                
                if( type == "data" )
                {
                    sampleContainer.isData.push_back(true);
                    sampleContainer.isMC.push_back(false);
                }
                else if( type == "signal" )
                {
                    sampleContainer.isData.push_back(false);
                    sampleContainer.isMC.push_back(false);
                }
                else if( type == "mc" )
                {
                    sampleContainer.isData.push_back(false);
                    sampleContainer.isMC.push_back(true);
                }
                else
                {
                    cerr << "Type unknown in samples configuration file, options: \"data\"/\"signal\"/\"mc\"" << endl;
                    throw 1;
                }
            }
            else 
            {
                if( type != previousType )
                {
                    cerr << "Samples within same reduced name group, but not of same type." << endl;
                    throw 1;
                }
            }
        }
        else 
        {
            if( newSuperSample )
                sampleContainer.color.push_back( samples[iSample] );
            else if( ((int) samples[iSample]) != ((int) samples[iSample-valuesPerLine]) )
            {
                cerr << "Samples within same reduced name group, but not with same color." << endl;
                throw 1;
            }
        }
    }
}

SampleReader::~SampleReader() { }