#include "libconfig.h++"
#include "EventContainer.hpp"
#include "EventReader.hpp"
#include "ConfigHandler.hpp"
#include "EventCleaner.hpp"
#include "EventSelecter.hpp"
#include "EventPlotter.hpp"

int main (int argc, char ** argv) {
    
    // check number of input parameters
    if(argc < 2){
        cerr<<"Needs the cfg file as agument --> exit "<<endl;
        return -1;
    }
        
    string cfgName(argv[1]);
    ConfigContainer cfgContainer;
    ConfigHandler* cHandler = nullptr;
    
    try
    {
        cHandler = new ConfigHandler(cfgName, cfgContainer);
        cHandler->readConfig();
    }
    catch(...)
    {
        cout << "exit program" << endl;
        if(cHandler) delete cHandler;
        return(-1);
    }
    

    EventContainer eventContainer;
    
    // Set EventReader and extra branches
    EventReader reader(eventContainer, cfgContainer);
    vector<string> readBranches = {"std_vector_LHE_weight"};
    reader.addReadBranches(readBranches);
    
//     EventCleaner cleaner(eventContainer);
    EventSelecter selecter(eventContainer, cfgContainer.cutContainer);
    
    // EventPlotter for each weight
    vector<string> qcdScaleStrings = {"mur_1_muf_1","mur_1_muf_2", "mur_1_muf_0p5", "mur_2_muf_1", "mur_2_muf_2", "mur_2_muf_0p5", "mur_0p5_muf_1", "mur_0p5_muf_2", "mur_0p5_muf_0p5"};    
    vector<EventPlotter*> plotterVector;
    string oldPlotString = cfgContainer.plotString;
    for( unsigned int iScale = 0; iScale < 9; ++iScale )
    {
        if( oldPlotString == "" )
            cfgContainer.plotString = qcdScaleStrings[iScale];
        else
            cfgContainer.plotString += ("_" + qcdScaleStrings[iScale]);
        
        plotterVector.push_back( new EventPlotter(eventContainer, cfgContainer) );
    }
    
    for( unsigned int iSample = 0; iSample < cfgContainer.sampleContainer.reducedNames.size(); ++iSample) 
    {
        for( unsigned int iSubSample = 0; iSubSample < cfgContainer.sampleContainer.sampleNames[iSample].size(); ++iSubSample) 
        {
            if( reader.setSample(iSample, iSubSample) )
            { 
                while( reader.fillNextEvent() )
                {
    //                 cleaner.doCleaning();
                    if( selecter.passCuts() )
                    {
                        for( unsigned int iScale = 0; iScale < 9; ++iScale )
                        {
                            reader.reweigh(iScale);
                            plotterVector[iScale]->fill(iSample, iSubSample);
                        }
                    }
                }
            }
        }
    }
//     string filename = "test.root";
//     plotter.writeHist(filename);
    for( unsigned int iScale = 0; iScale < 9; ++iScale )
    {
        if( oldPlotString == "" )
            cfgContainer.plotString = qcdScaleStrings[iScale];
        else
            cfgContainer.plotString += ("_" + qcdScaleStrings[iScale]);
        
        plotterVector[iScale]->writePlots("png");
    }
    
    delete cHandler;
    for( unsigned int iScale = 0; iScale < 9; ++iScale )
    {
        delete plotterVector[iScale];
    }
}
