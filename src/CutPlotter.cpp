
#include "CutPlotter.hpp"

CutPlotter::CutPlotter(const EventContainer& evContainer, const ConfigContainer& cfgContainer)
: BasePlotter(evContainer, cfgContainer), nSamples(configContainer.sampleContainer.reducedNames.size()), nCuts(configContainer.cutContainer.variableNames.size()), histogramContainer("Cut_events", nSamples), numberOfEntriesVector(nSamples, vector<unsigned int>(nCuts+1, 0))
{  
    // Set histogramContainer
    for( unsigned int iSample = 0; iSample < nSamples; ++iSample )
    {
        if( configContainer.plotString != "" )
        {
            histogramContainer.containerName.append("_");
            histogramContainer.containerName.append(configContainer.plotString);
        }
        
        histogramContainer.histograms.push_back( new TH1F((configContainer.sampleContainer.reducedNames[iSample] + "_" + histogramContainer.containerName).c_str(), "", nCuts, 0, nCuts) );
        histogramContainer.reducedNames.push_back(configContainer.sampleContainer.reducedNames[iSample]);
        histogramContainer.color.push_back(configContainer.sampleContainer.color[iSample]);
        histogramContainer.sampleType.push_back(configContainer.sampleContainer.sampleType[iSample]);

        // Set histrogram labels
        for( unsigned int iCut = 0; iCut < nCuts; ++iCut ) 
        {  
            histogramContainer.histograms[iSample]->GetXaxis()->SetBinLabel(iCut+1, (configContainer.cutContainer.variableNames[iCut]).c_str());
        }
        histogramContainer.histograms[iSample]->GetYaxis()->SetTitle("Events");
    }
}

CutPlotter::~CutPlotter(){ }

void CutPlotter::fill(unsigned int iSample, unsigned int iSubSample, int iCut) 
{
    if( iSample < nSamples && iSubSample < configContainer.sampleContainer.sampleNames[iSample].size() && iCut < (signed) nCuts )
    {
        if( configContainer.sampleContainer.sampleType[iSample] == SampleType::DATA )
        {
            histogramContainer.histograms[iSample]->Fill(iCut);
        }
        else if( configContainer.sampleContainer.sampleType[iSample] == SampleType::FAKELEPTON )
        {
            histogramContainer.histograms[iSample]->Fill(iCut, eventWeightFunction());
        }
        else
        {
            histogramContainer.histograms[iSample]->Fill( iCut, globalWeight[iSample][iSubSample]*eventWeightFunction() );
        }
        
        if( iCut >= 0 )
            numberOfEntriesVector[iSample][iCut+1]++;
        else
            numberOfEntriesVector[iSample][0]++;
    }
    else
    {
        cerr << "Indices for sample: "<< iSample << ", subsample: " << iSubSample << " or iCut: " << iCut << " out of range in the CutPlotter::fill function." << endl;
        throw 1;
    }
}

void CutPlotter::fillTotal(unsigned int iSample, unsigned int iSubSample)
{
    fill(iSample, iSubSample, -1);
}

void CutPlotter::writeHist(string filename)
{
    BasePlotter::writeHist(filename, histogramContainer.histograms, "RECREATE");
}

void CutPlotter::writeStacked(string extension)
{
    histogramContainer.containerName = "Cut_events";
    if( configContainer.plotString != "" )
    {
        histogramContainer.containerName.append("_");
        histogramContainer.containerName.append(configContainer.plotString);
    }
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
        float totEvents = histogramContainer.histograms[iSample]->GetBinContent(0);
        
        // Check negative values for fake lepton samples
        if( histogramContainer.sampleType[iSample] == SampleType::FAKELEPTON ) 
        {
            float maximum = histogramContainer.histograms[iSample]->GetMaximum();
            if( maximum > totEvents || isnan(totEvents) ) totEvents = maximum;
            
            unsigned int nBins = histogramContainer.histograms[iSample]->GetNbinsX();
            for( unsigned int iBin=0; iBin < nBins; ++iBin )
            {
                if( histogramContainer.histograms[iSample]->GetBinContent(iBin+1) < 0. )
                    histogramContainer.histograms[iSample]->SetBinContent(iBin+1, 0.);
            }
        }
                
        if( totEvents > 0. )
        {
            hDenom.push_back( new TH1F(("eff_denominator_" + configContainer.sampleContainer.reducedNames[iSample]).c_str(),"", nCuts, 0, nCuts) );
            for( unsigned int iCut = 0; iCut <= nCuts+1; ++iCut ) 
            {
                hDenom[iSample]->SetBinContent(iCut, totEvents);
            }
        }
        else if( configContainer.sampleContainer.sampleType[iSample] == SampleType::DATA && ! configContainer.unblind )
        {
            hDenom.push_back( new TH1F(("eff_denominator_" + configContainer.sampleContainer.reducedNames[iSample]).c_str(),"", nCuts, 0, nCuts) );
            for( unsigned int iCut = 0; iCut <= nCuts+1; ++iCut ) 
            {
                hDenom[iSample]->SetBinContent(iCut, 1);
            }
        }
        else 
        {
            cerr << "Sample \"" << configContainer.sampleContainer.reducedNames[iSample] << "\" has no events, cannot make efficiency plot" << endl;
            throw 1;
        }
    }
    
    BasePlotter::writeEfficiency(histogramContainer, hDenom, extension, binNames);
}

void CutPlotter::printEvents() const
{
    unsigned int coutWidth = 20;
    cout << setprecision(2) << fixed << endl;
    
    // Number of events
    cout << "Number of events after cuts" << endl;
    cout << setw(coutWidth) << " ";
    for( unsigned int iSample = 0; iSample < nSamples; ++iSample )
    {
        cout << setw(coutWidth) << configContainer.sampleContainer.reducedNames[iSample];
    }
    cout << endl;
    
    for( unsigned int iCut = 0; iCut <= nCuts; ++iCut ) 
    {
        if( iCut )
            cout << setw(coutWidth) << configContainer.cutContainer.variableNames[iCut-1];
        else
            cout << setw(coutWidth) << "Total";
        for( unsigned int iSample = 0; iSample < nSamples; ++iSample )
        {
            cout << setw(coutWidth) << histogramContainer.histograms[iSample]->GetBinContent(iCut);
        }
        cout << endl;
    }
    cout << endl;
    
    // Number of entries
    cout << "Number of entries after cuts" << endl;
    cout << setw(coutWidth) << " ";
    for( unsigned int iSample = 0; iSample < nSamples; ++iSample )
    {
        cout << setw(coutWidth) << configContainer.sampleContainer.reducedNames[iSample];
    }
    cout << endl;

    for( unsigned int iCut = 0; iCut <= nCuts; ++iCut ) 
    {
        if( iCut )
            cout << setw(coutWidth) << configContainer.cutContainer.variableNames[iCut-1];
        else
            cout << setw(coutWidth) << "Total";
        for( unsigned int iSample = 0; iSample < nSamples; ++iSample )
        {
            cout << setw(coutWidth) << numberOfEntriesVector[iSample][iCut];
        }
        cout << endl;
    }
    cout << endl;
}