 
#include "EventPlotter.hpp"

EventPlotter::EventPlotter(const EventContainer& evContainer, const ConfigContainer& cfgContainer)
: BasePlotter(evContainer, cfgContainer), nSamples(configContainer.sampleContainer.reducedNames.size()), nVariables(configContainer.variableContainer.variableNames.size()), nHistograms(0)
{    
    // Set functions to variables
    VariableDictionary varDictionary(eventContainer);
    varDictionary.stringToFunction(configContainer.variableContainer.variableNames, functionVector);
       
    // Initialize histogramContainers
    for( unsigned int iVar = 0; iVar < nVariables; ++iVar )
    {
        string histContName = configContainer.variableContainer.variableNames[iVar];
        if( configContainer.variableContainer.is2D[iVar] ) histContName += ("_" +  configContainer.variableContainer.variableNames[iVar+1]);
            
        if( configContainer.plotString != "" )
        {
            histContName.append("_");
            histContName.append(configContainer.plotString);
        }
        
        histogramContainers.push_back( HistogramContainer(histContName, nSamples) );
        for( unsigned int iSample = 0; iSample < nSamples; ++iSample )
        {
            if( configContainer.variableContainer.is2D[iVar] )
            {
                if( configContainer.variableContainer.binning[iVar].size() == 2 && configContainer.variableContainer.binning[iVar+1].size() == 2 )
                    histogramContainers.back().histograms.push_back( new TH2D((configContainer.sampleContainer.reducedNames[iSample] + "_" + histogramContainers.back().containerName).c_str(), "", configContainer.variableContainer.nBins[iVar], configContainer.variableContainer.binning[iVar][0], configContainer.variableContainer.binning[iVar][1],
                    configContainer.variableContainer.nBins[iVar+1], configContainer.variableContainer.binning[iVar+1][0], configContainer.variableContainer.binning[iVar+1][1]) ) ;
                else if( configContainer.variableContainer.binning[iVar].size() == 2 && configContainer.variableContainer.binning[iVar+1].size() != 2 )
                    histogramContainers.back().histograms.push_back( new TH2D((configContainer.sampleContainer.reducedNames[iSample] + "_" + histogramContainers.back().containerName).c_str(), "", configContainer.variableContainer.nBins[iVar], configContainer.variableContainer.binning[iVar][0], configContainer.variableContainer.binning[iVar][1],
                    configContainer.variableContainer.nBins[iVar+1], &configContainer.variableContainer.binning[iVar+1][0]) ) ;
                else if( configContainer.variableContainer.binning[iVar+1].size() == 2 )
                    histogramContainers.back().histograms.push_back( new TH2D((configContainer.sampleContainer.reducedNames[iSample] + "_" + histogramContainers.back().containerName).c_str(), "", configContainer.variableContainer.nBins[iVar], &configContainer.variableContainer.binning[iVar][0],
                    configContainer.variableContainer.nBins[iVar+1], configContainer.variableContainer.binning[iVar+1][0], configContainer.variableContainer.binning[iVar+1][1]) ) ;
                else
                    histogramContainers.back().histograms.push_back( new TH2D((configContainer.sampleContainer.reducedNames[iSample] + "_" + histogramContainers.back().containerName).c_str(), "", configContainer.variableContainer.nBins[iVar], &configContainer.variableContainer.binning[iVar][0],
                    configContainer.variableContainer.nBins[iVar+1], &configContainer.variableContainer.binning[iVar+1][0]) ) ;
                
                histogramContainers.back().histograms[iSample]->GetYaxis()->SetTitle(configContainer.variableContainer.variableNames[iVar+1].c_str());
            }
            else
            {
                if( configContainer.variableContainer.binning[iVar].size() == 2 )
                {
                    histogramContainers.back().histograms.push_back( new TH1D((configContainer.sampleContainer.reducedNames[iSample] + "_" + histogramContainers.back().containerName).c_str(), "", configContainer.variableContainer.nBins[iVar], configContainer.variableContainer.binning[iVar][0], configContainer.variableContainer.binning[iVar][1]) ) ;
                }
                else
                {
                    histogramContainers.back().histograms.push_back( new TH1D((configContainer.sampleContainer.reducedNames[iSample] + "_" + histogramContainers.back().containerName).c_str(), "", configContainer.variableContainer.nBins[iVar], &configContainer.variableContainer.binning[iVar][0]) );
                }
            }
            histogramContainers.back().histograms[iSample]->GetXaxis()->SetTitle(configContainer.variableContainer.variableNames[iVar].c_str());
//             histVector[iVar].StatOverflows(1); // use under and overflow for mean, rms ...
            histogramContainers.back().reducedNames.push_back(configContainer.sampleContainer.reducedNames[iSample]);
            histogramContainers.back().color.push_back(configContainer.sampleContainer.color[iSample]);
            histogramContainers.back().sampleType.push_back(configContainer.sampleContainer.sampleType[iSample]);
        }
        if( configContainer.variableContainer.axisRanges.find(iVar) != configContainer.variableContainer.axisRanges.end() )
            histogramContainers.back().axisRanges = configContainer.variableContainer.axisRanges.at(iVar);
                
        if( configContainer.variableContainer.is2D[iVar] )
            iVar++;
    }
    nHistograms = histogramContainers.size();
}

EventPlotter::~EventPlotter(){ }

void EventPlotter::fill(unsigned int iSample, unsigned int iSubSample) 
{
    if( iSample < nSamples && iSubSample < configContainer.sampleContainer.sampleNames[iSample].size() )
    {
        if( configContainer.sampleContainer.sampleType[iSample] == SampleType::DATA )
        {
            for( unsigned int iVar = 0, iHist = 0; iVar < nVariables; ++iVar, ++iHist )
            {
                if( configContainer.variableContainer.is2D[iVar] )
                {
                    TH2D* h = (TH2D*) histogramContainers[iHist].histograms[iSample];
                    h->Fill( functionVector[iVar](), functionVector[iVar+1]() );
                    iVar++;
                }
                else
                    histogramContainers[iHist].histograms[iSample]->Fill( functionVector[iVar]() );
            }
        }
        else if( configContainer.sampleContainer.sampleType[iSample] == SampleType::FAKELEPTON )
        {
            for( unsigned int iVar = 0, iHist = 0; iVar < nVariables; ++iVar, ++iHist )
            {
                if( configContainer.variableContainer.is2D[iVar] )
                {
                    TH2D* h = (TH2D*) histogramContainers[iHist].histograms[iSample];
                    h->Fill( functionVector[iVar](), functionVector[iVar+1](), eventWeightFunction() );
                    iVar++;
                }
                else
                    histogramContainers[iHist].histograms[iSample]->Fill( functionVector[iVar](), eventWeightFunction() );
            }
        }
        else
        {
            for( unsigned int iVar = 0, iHist = 0; iVar < nVariables; ++iVar, ++iHist )
            {
                if( configContainer.variableContainer.is2D[iVar] )
                {
                    TH2D* h = (TH2D*) histogramContainers[iHist].histograms[iSample];
                    h->Fill( functionVector[iVar](), functionVector[iVar+1](), globalWeight[iSample][iSubSample]*eventWeightFunction() );
                    iVar++;
                }
                else
                    histogramContainers[iHist].histograms[iSample]->Fill( functionVector[iVar](), globalWeight[iSample][iSubSample]*eventWeightFunction() );
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
    for( unsigned int iVar = 0; iVar < nHistograms; ++iVar )
    {    
        if( configContainer.addOverflow ) 
        {
            histogramContainers[iVar].addOverflow();
        }
        if( configContainer.addUnderflow ) 
        {
            histogramContainers[iVar].addUnderflow();
        }
        if( iVar == 0 )
            BasePlotter::writeHist(filename, histogramContainers[iVar].histograms, "RECREATE");
        else
            BasePlotter::writeHist(filename, histogramContainers[iVar].histograms, "UPDATE");
    }
}

void EventPlotter::writePlots(string extension)
{
    for( unsigned int iVar = 0; iVar < nHistograms; ++iVar )
    {
        if( configContainer.addOverflow ) 
        {
            histogramContainers[iVar].addOverflow();
        }
        if( configContainer.addUnderflow ) 
        {
            histogramContainers[iVar].addUnderflow();
        }
        BasePlotter::writeStacked("Stack_plots.root", histogramContainers[iVar], extension);        
    }
}

HistogramContainer* EventPlotter::getHistogramContainer(unsigned int i)
{
    if( i < histogramContainers.size() )
        return &(histogramContainers[i]);
    else
        cerr << "No histogram with index: " << i << endl;
        throw 1;
}

// parallel running
// EventPlotter::EventPlotter(const ConfigContainer& cfgContainer)
// : BasePlotter(EventContainer(), cfgContainer), nSamples(configContainer.sampleContainer.reducedNames.size()), nVariables(configContainer.variableContainer.variableNames.size())
// {
//     // Set style
//     LatinoStyle2();
//     
//     // Create parallel vectors
//     functionVectorParallel.resize(nSamples);
//     eventWeightFunctionParallel.resize(nSamples);
//     eventContainerParallel.resize(nSamples);
//        
//     // Initialize TH1Ds
//     for( unsigned int iSample = 0; iSample < nSamples; ++iSample )
//     {
//         vector<TH1D*> histVector;
//         for( unsigned int iVar = 0; iVar < nVariables; ++iVar )
//         {
//             if( configContainer.variableContainer.binning[iVar].size() == 2 )
//             {
//                 histVector.push_back(new TH1D((configContainer.sampleContainer.reducedNames[iSample] + "_" + configContainer.variableContainer.variableNames[iVar]).c_str(), "", configContainer.variableContainer.nBins[iVar], configContainer.variableContainer.binning[iVar][0], configContainer.variableContainer.binning[iVar][1]));
//             }
//             else
//             {
//                 histVector.push_back(new TH1D((configContainer.sampleContainer.reducedNames[iSample] + "_" + configContainer.variableContainer.variableNames[iVar]).c_str(), "", configContainer.variableContainer.nBins[iVar], &configContainer.variableContainer.binning[iVar][0]));
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
//         if( configContainer.sampleContainer.sampleType[iSample] == SampleType::DATA )
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