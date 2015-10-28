
#include "CutPlotter.hpp"

CutPlotter::CutPlotter(const EventContainer& evContainer, const ConfigContainer& cfgContainer)
: BasePlotter(evContainer, cfgContainer), nSamples(configContainer.sampleContainer.reducedNames.size()), nCuts(configContainer.cutContainer.variableNames.size()), histogramContainer("Cut_events", nSamples)
{
    
    // Set histogramContainer
    for( unsigned int iSample = 0; iSample < nSamples; ++iSample )
    {
        histogramContainer.histograms.push_back( new TH1F(("Cut_Efficiency_" + configContainer.sampleContainer.reducedNames[iSample]).c_str(), "", nCuts, 0, nCuts) );
        histogramContainer.reducedNames.push_back(configContainer.sampleContainer.reducedNames[iSample]);
        histogramContainer.color.push_back(configContainer.sampleContainer.color[iSample]);
        histogramContainer.isData.push_back(configContainer.sampleContainer.isData[iSample]);
        histogramContainer.isMC.push_back(configContainer.sampleContainer.isMC[iSample]);

        // Set histrogram labels
        for( unsigned int iCut = 0; iCut < nCuts; ++iCut ) 
        {  
            histogramContainer.histograms[iSample]->GetXaxis()->SetBinLabel(iCut+1, (configContainer.cutContainer.variableNames[iCut]).c_str());
        }
        histogramContainer.histograms[iSample]->GetYaxis()->SetTitle("Events");
    }
}

CutPlotter::~CutPlotter(){ }

void CutPlotter::fill(unsigned int iSample, unsigned int iSubSample, unsigned int iCut) 
{
    if( iSample < nSamples && iSubSample < configContainer.sampleContainer.sampleNames[iSample].size() && iCut <= nCuts )
    {
        if( configContainer.sampleContainer.isData[iSample] )
        {
            histogramContainer.histograms[iSample]->Fill(iCut);
        }
        else
        {
            histogramContainer.histograms[iSample]->Fill( iCut, globalWeight[iSample][iSubSample]*eventWeightFunction() );
        }
    }
    else
    {
        cerr << "Indices for sample, subsample or iCut out of range in the CutPlotter::fill function." << endl;
        throw 1;
    }
}

void CutPlotter::writeHist(string filename) const
{
    BasePlotter::writeHist(filename, histogramContainer.histograms, "RECREATE");
}

void CutPlotter::writeStacked(string extension)
{
    histogramContainer.containerName = "Cut_events";
    BasePlotter::writeStacked(histogramContainer, extension);
}

void CutPlotter::writeEfficiency(string extension)
{
    histogramContainer.containerName = "Cut_efficiency";
    vector<string> binNames;
    for( unsigned int iCut = 0; iCut < nCuts; ++iCut ) 
    {  
        binNames.push_back( (configContainer.cutContainer.variableNames[iCut]).c_str() );
    }
    
    // Get denominator
    vector<TH1*> hDenom;
    for( unsigned int iSample = 0; iSample < nSamples; ++iSample )
    {
        float totEvents = histogramContainer.histograms[iSample]->GetBinContent(nCuts+1);
        
        if( totEvents > 0 )
        {
            hDenom.push_back( new TH1F(("eff_denominator_" + configContainer.sampleContainer.reducedNames[iSample]).c_str(),"", nCuts, 0, nCuts) );
            for( unsigned int iCut = 0; iCut <= nCuts; ++iCut ) 
            {
                hDenom[iSample]->SetBinContent(iCut+1, totEvents);
            }
        }
        else
        {
            cerr << "Sample \"" << configContainer.sampleContainer.reducedNames[iSample] << "\" has no events, cannot make efficiency plot";
            throw 1;
        }
    }
    
    BasePlotter::writeEfficiency(histogramContainer, hDenom, extension, binNames);
}