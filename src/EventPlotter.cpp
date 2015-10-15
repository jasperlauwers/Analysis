
#include "EventPlotter.hpp"

EventPlotter::EventPlotter(const EventContainer& evContainer, const ConfigContainer& cfgContainer)
: eventContainer(evContainer), configContainer(cfgContainer), nSamples(configContainer.sampleContainer.reducedNames.size()), nVariables(configContainer.variableContainer.variableNames.size())
{
    // Set functions to variables
    VariableDictionary varDictionary(eventContainer);
    varDictionary.stringToFunction(configContainer.variableContainer.variableNames, functionVector);
       
    // Initialize TH1Fs
    for( unsigned int iSample = 0; iSample < nSamples; ++iSample )
    {
        vector<TH1F*> histVector;
        for( unsigned int iVar = 0; iVar < nVariables; ++iVar )
        {
            if( configContainer.variableContainer.binning[iVar].size() == 2 )
            {
                cout << configContainer.variableContainer.binning[iVar][0] << " " << configContainer.variableContainer.binning[iVar][1] << endl;
                histVector.push_back(new TH1F((configContainer.sampleContainer.reducedNames[iSample] + "_" + configContainer.variableContainer.variableNames[iVar]).c_str(), "", configContainer.variableContainer.nBins[iVar], configContainer.variableContainer.binning[iVar][0], configContainer.variableContainer.binning[iVar][1]));
            }
            else
            {
                histVector.push_back(new TH1F((configContainer.sampleContainer.reducedNames[iSample] + "_" + configContainer.variableContainer.variableNames[iVar]).c_str(), "", configContainer.variableContainer.nBins[iVar], &configContainer.variableContainer.binning[iVar][0]));
            }
            histVector[iVar]->GetXaxis()->SetTitle(configContainer.variableContainer.variableNames[iVar].c_str());
//             histVector[iVar].StatOverflows(1); // use under and overflow for mean, rms ...
            histVector[iVar]->Sumw2();
        }
        histograms.push_back(histVector);
    }
    
    // Set globalWeight vector
    for( unsigned int iSample = 0; iSample < nSamples; ++iSample )
    {
        vector<float> weightVector;
        unsigned int nSubSamples = configContainer.sampleContainer.sampleNames[iSample].size();
        for( unsigned int iSubSample = 0; iSubSample < nSubSamples; ++iSubSample )
        {
            weightVector.push_back(configContainer.luminosity / configContainer.sampleContainer.luminosity[iSample][iSubSample] * configContainer.sampleContainer.weight[iSample][iSubSample]);
        }
        globalWeight.push_back(weightVector);
    }
    
    // Set weight function
    eventWeightFunction = bind(&EventContainer::weight, &eventContainer);
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
                histograms[iSample][iVar]->Fill( functionVector[iVar]() );
            }
        }
        else
        {
            for( unsigned int iVar = 0; iVar < nVariables; ++iVar )
            {
                histograms[iSample][iVar]->Fill( functionVector[iVar](), globalWeight[iSample][iSubSample]*eventWeightFunction() );
            }
        }
    }
    else
    {
        cerr << "Indices for sample or subsample out of range in the EventPlotter::fill function." << endl;
        throw 1;
    }
}

void EventPlotter::writeHist(string filename) const
{
    if( filename.find(".root") == string::npos )
        filename.append( ".root" );
    
    system(("mkdir -p " + configContainer.outputDir).c_str());   
    TFile* f = new TFile((configContainer.outputDir + filename).c_str(),"RECREATE");
    
    for( unsigned int iSample = 0; iSample < nSamples; ++iSample )
    {
        for( unsigned int iVar = 0; iVar < nVariables; ++iVar )
        {
            histograms[iSample][iVar]->Write();
        }
    }
    f->Close();
}

void EventPlotter::writePlots(string extension) const
{
    if( extension[0] == '.' )
        extension.erase(0,1);
    
    system(("mkdir -p " + configContainer.outputDir + extension).c_str());  
    
    for( unsigned int iVar = 0; iVar < nVariables; ++iVar )
    {
        TCanvas *c = new TCanvas(configContainer.variableContainer.variableNames[iVar].c_str());
        TLegend *leg = new TLegend(0.73,0.25,0.88,0.35); 
        leg->SetTextSize(0.05);
        THStack *hs = new THStack(("stack_"+configContainer.variableContainer.variableNames[iVar]).c_str(),"");
        TH1F *hData = nullptr;
        
        // Add backgrounds to stack
        for( unsigned int iSample = 0; iSample < nSamples; ++iSample )
        {
            if( configContainer.sampleContainer.isMC[iSample] )
            {
                histograms[iSample][iVar]->SetLineColor(configContainer.sampleContainer.color[iSample]);
                histograms[iSample][iVar]->SetFillColor(configContainer.sampleContainer.color[iSample]);
                hs->Add(histograms[iSample][iVar]);
                leg->AddEntry(histograms[iSample][iVar],configContainer.sampleContainer.reducedNames[iSample].c_str(),"fl");
            }
        }
        
        // Add signal to stack
        for( unsigned int iSample = 0; iSample < nSamples; ++iSample )
        {
            if( ! configContainer.sampleContainer.isMC[iSample] && ! configContainer.sampleContainer.isData[iSample] )
            {
                histograms[iSample][iVar]->SetLineColor(configContainer.sampleContainer.color[iSample]);
                histograms[iSample][iVar]->SetFillColor(configContainer.sampleContainer.color[iSample]);
                hs->Add(histograms[iSample][iVar]);
                leg->AddEntry(histograms[iSample][iVar],configContainer.sampleContainer.reducedNames[iSample].c_str(),"fl");
            }
        }
        
        // Add data
        for( unsigned int iSample = 0; iSample < nSamples; ++iSample )
        {
            if( configContainer.sampleContainer.isData[iSample] )
            {
                histograms[iSample][iVar]->SetLineColor(configContainer.sampleContainer.color[iSample]);
                histograms[iSample][iVar]->SetMarkerColor(configContainer.sampleContainer.color[iSample]);
                histograms[iSample][iVar]->SetMarkerSize(2);
                leg->AddEntry(histograms[iSample][iVar],configContainer.sampleContainer.reducedNames[iSample].c_str(),"lep");
                hData = histograms[iSample][iVar];
            }
        }
        
        // Draw
        hs->Draw();
        hData->Draw("elp same");
        leg->Draw(); 
        c->Print((configContainer.outputDir + extension + "/" + configContainer.variableContainer.variableNames[iVar] + "." + extension).c_str(), extension.c_str());
    }
}

unsigned int EventPlotter::getIndex(const string& indexString, const string& fullString, bool isJet)
{
    if( indexString.size() != 1 )
    {
        cerr << "Missing index for variable: " << fullString << endl;
        throw 1;
    }
    
    unsigned int index = (unsigned) stoi(indexString);
    
    if( index == (unsigned) 0 )
    {
        cerr << "Index 0 for variable: " << fullString << endl;
        throw 1;        
    }
    
    // jet1 = jet with index 0
    index--;
    return index;      
}