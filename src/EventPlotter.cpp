 
#include "EventPlotter.hpp"

EventPlotter::EventPlotter(const EventContainer& evContainer, const ConfigContainer& cfgContainer)
: BasePlotter(evContainer, cfgContainer), nSamples(configContainer.sampleContainer.reducedNames.size()), nVariables(configContainer.variableContainer.variableNames.size())
{    
    // Set functions to variables
    VariableDictionary varDictionary(eventContainer);
    varDictionary.stringToFunction(configContainer.variableContainer.variableNames, functionVector);
       
    // Initialize histogramContainers
    for( unsigned int iVar = 0; iVar < nVariables; ++iVar )
    {
        histogramContainers.push_back( HistogramContainer((configContainer.variableContainer.variableNames[iVar]).c_str(), nSamples) );
        for( unsigned int iSample = 0; iSample < nSamples; ++iSample )
        {
            if( configContainer.variableContainer.binning[iVar].size() == 2 )
            {
                histogramContainers[iVar].histograms.push_back( new TH1F((configContainer.sampleContainer.reducedNames[iSample] + "_" + configContainer.variableContainer.variableNames[iVar]).c_str(), "", configContainer.variableContainer.nBins[iVar], configContainer.variableContainer.binning[iVar][0], configContainer.variableContainer.binning[iVar][1]) ) ;
            }
            else
            {
                histogramContainers[iVar].histograms.push_back( new TH1F((configContainer.sampleContainer.reducedNames[iSample] + "_" + configContainer.variableContainer.variableNames[iVar]).c_str(), "", configContainer.variableContainer.nBins[iVar], &configContainer.variableContainer.binning[iVar][0]) );
            }
            histogramContainers[iVar].histograms[iSample]->GetXaxis()->SetTitle(configContainer.variableContainer.variableNames[iVar].c_str());
//             histVector[iVar].StatOverflows(1); // use under and overflow for mean, rms ...
            histogramContainers[iVar].reducedNames.push_back(configContainer.sampleContainer.reducedNames[iSample]);
            histogramContainers[iVar].color.push_back(configContainer.sampleContainer.color[iSample]);
            histogramContainers[iVar].isData.push_back(configContainer.sampleContainer.isData[iSample]);
            histogramContainers[iVar].isMC.push_back(configContainer.sampleContainer.isMC[iSample]);
        }
    }
}

EventPlotter::~EventPlotter(){ }

void EventPlotter::fill(unsigned int iSample, unsigned int iSubSample) 
{
    if( iSample < nSamples && iSubSample < configContainer.sampleContainer.sampleNames[iSample].size() )
    {
        if( configContainer.sampleContainer.isData[iSample] )
        {
            for( unsigned int iVar = 0; iVar < nVariables; ++iVar )
            {
                histogramContainers[iVar].histograms[iSample]->Fill( functionVector[iVar]() );
            }
        }
        else
        {
            for( unsigned int iVar = 0; iVar < nVariables; ++iVar )
            {
                histogramContainers[iVar].histograms[iSample]->Fill( functionVector[iVar](), globalWeight[iSample][iSubSample]*eventWeightFunction() );
            }
        }
    }
    else
    {
        cerr << "Indices for sample or subsample out of range in the EventPlotter::fill function." << endl;
        throw 1;
    }
}

void EventPlotter::writeHist(string filename)
{    
    for( unsigned int iVar = 0; iVar < nVariables; ++iVar )
    {    
        if( configContainer.addOverflow ) 
        {
            histogramContainers[iVar].addOverflow();
        }
        if( iVar == 0 )
            BasePlotter::writeHist(filename, histogramContainers[iVar].histograms, "RECREATE");
        else
            BasePlotter::writeHist(filename, histogramContainers[iVar].histograms, "UPDATE");
    }
}

void EventPlotter::writePlots(string extension)
{
    for( unsigned int iVar = 0; iVar < nVariables; ++iVar )
    {
        if( configContainer.addOverflow ) 
        {
            histogramContainers[iVar].addOverflow();
        }
        BasePlotter::writeStacked(histogramContainers[iVar], extension);        
    }
}

// parallel running
// EventPlotter::EventPlotter(const ConfigContainer& cfgContainer)
// : eventContainer( EventContainer() ), configContainer(cfgContainer), nSamples(configContainer.sampleContainer.reducedNames.size()), nVariables(configContainer.variableContainer.variableNames.size())
// {
//     // Set style
//     LatinoStyle2();
//     
//     // Create parallel vectors
//     functionVectorParallel.resize(nSamples);
//     eventWeightFunctionParallel.resize(nSamples);
//     eventContainerParallel.resize(nSamples);
//        
//     // Initialize TH1Fs
//     for( unsigned int iSample = 0; iSample < nSamples; ++iSample )
//     {
//         vector<TH1F*> histVector;
//         for( unsigned int iVar = 0; iVar < nVariables; ++iVar )
//         {
//             if( configContainer.variableContainer.binning[iVar].size() == 2 )
//             {
//                 cout << configContainer.variableContainer.binning[iVar][0] << " " << configContainer.variableContainer.binning[iVar][1] << endl;
//                 histVector.push_back(new TH1F((configContainer.sampleContainer.reducedNames[iSample] + "_" + configContainer.variableContainer.variableNames[iVar]).c_str(), "", configContainer.variableContainer.nBins[iVar], configContainer.variableContainer.binning[iVar][0], configContainer.variableContainer.binning[iVar][1]));
//             }
//             else
//             {
//                 histVector.push_back(new TH1F((configContainer.sampleContainer.reducedNames[iSample] + "_" + configContainer.variableContainer.variableNames[iVar]).c_str(), "", configContainer.variableContainer.nBins[iVar], &configContainer.variableContainer.binning[iVar][0]));
//             }
//             histVector[iVar]->GetXaxis()->SetTitle(configContainer.variableContainer.variableNames[iVar].c_str());
// //             histVector[iVar]->GetYaxis()->SetTitle("events");
// //             histVector[iVar].StatOverflows(1); // use under and overflow for mean, rms ...
//         }
//         histograms.push_back(histVector);
//     }
//     
//     // Set globalWeight vector
//     for( unsigned int iSample = 0; iSample < nSamples; ++iSample )
//     {
//         vector<float> weightVector;
//         unsigned int nSubSamples = configContainer.sampleContainer.sampleNames[iSample].size();
//         for( unsigned int iSubSample = 0; iSubSample < nSubSamples; ++iSubSample )
//         {
//             weightVector.push_back(configContainer.luminosity / configContainer.sampleContainer.luminosity[iSample][iSubSample] * configContainer.sampleContainer.weight[iSample][iSubSample]);
//         }
//         globalWeight.push_back(weightVector);
//     }
// }
// 
// void EventPlotter::init(const EventContainer& evContainer, unsigned int iSample)
// {
//     eventContainerParallel[iSample] = &evContainer;
//     
//     // Set functions to variables
//     VariableDictionary varDictionary(*eventContainerParallel[iSample]);
//     varDictionary.stringToFunction(configContainer.variableContainer.variableNames, functionVectorParallel[iSample]);
//     
//     // Set weight function
//     eventWeightFunctionParallel[iSample] = bind( &EventContainer::weight, eventContainerParallel[iSample] );
// }
// 
// void EventPlotter::parallelFill(unsigned int iSample, unsigned int iSubSample) 
// {
//     if( iSample < nSamples && iSubSample < configContainer.sampleContainer.sampleNames[iSample].size() )
//     {
//         if( configContainer.sampleContainer.isData[iSample] )
//         {
//             for( unsigned int iVar = 0; iVar < nVariables; ++iVar )
//             {
//                 histograms[iSample][iVar]->Fill( functionVectorParallel[iSample][iVar]() );
//             }
//         }
//         else
//         {
//             for( unsigned int iVar = 0; iVar < nVariables; ++iVar )
//             {
//                 histograms[iSample][iVar]->Fill( functionVectorParallel[iSample][iVar](), globalWeight[iSample][iSubSample]*eventWeightFunctionParallel[iSample]() );
//             }
//         }
//     }
//     else
//     {
//         cerr << "Indices for sample or subsample out of range in the EventPlotter::fill function." << endl;
//         throw 1;
//     }
// }