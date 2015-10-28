
#include "BasePlotter.hpp"

BasePlotter::BasePlotter(const EventContainer& evContainer, const ConfigContainer& cfgContainer)
: eventContainer(evContainer), configContainer(cfgContainer)
{
    // Set Latinos style
    LatinoStyle2();
    
    // Set Sumw2
    TH1::SetDefaultSumw2();
    
        
    // Set globalWeight vector
    unsigned int nSamples = configContainer.sampleContainer.reducedNames.size();
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

BasePlotter::~BasePlotter() { }

void BasePlotter::writeHist(string filename, const vector<TH1*>& histVector, string option) const
{
    if( filename.find(".root") == string::npos )
        filename.append( ".root" );
    
    system(("mkdir -p " + configContainer.outputDir).c_str());   
    TFile* f = new TFile((configContainer.outputDir + filename).c_str(), option.c_str());
    
    for( const auto hist : histVector )
    {
        hist->Write();
    }
    f->Close();
}

void BasePlotter::writeStacked(const HistogramContainer& histContainer, string extension) const
{
    // Check histContainer consistency
    if( ! histContainer.check() ) 
        throw 1;

    
    if( extension[0] == '.' )
        extension.erase(0,1);
    
    system(("mkdir -p " + configContainer.outputDir).c_str());  
    
    unsigned int nSamples = histContainer.reducedNames.size();
    TCanvas *c = new TCanvas(("Canv_stacked_" + histContainer.containerName).c_str(), "", 600, 600);
    TLegend* leg = nullptr;
    vector<TLatex*> latexVector;
    setCanvasAttributes( nSamples, leg, latexVector);
    
    vector<TH1F*> hStack;
    TH1* hData = nullptr;
    
    // Add backgrounds to stack
    for( int iSample = nSamples-1; iSample > -1; --iSample )
    {
        if( histContainer.isMC[iSample] )
        {
            TH1F *temp = (TH1F*) histContainer.histograms[iSample]->Clone(("stack_hist_"+histContainer.reducedNames[iSample]).c_str());
            if( iSample < (signed) nSamples - 1 ) temp->Add(hStack[hStack.size()-1]);
            temp->SetLineColor(histContainer.color[iSample]);
            temp->SetFillColor(histContainer.color[iSample]);
            temp->SetFillStyle(1001);
            hStack.push_back(temp);
            leg->AddEntry(temp,histContainer.reducedNames[iSample].c_str(),"f");
        }
    }
    
    // Add signal to stack
    for( unsigned int iSample = 0; iSample < nSamples; ++iSample )
    {
        if( ! histContainer.isMC[iSample] && ! histContainer.isData[iSample] )
        {
            TH1F *temp = (TH1F*) histContainer.histograms[iSample]->Clone(("stack_hist_"+histContainer.reducedNames[iSample]).c_str());
            temp->Add(hStack[hStack.size()-1]);
            temp->SetLineColor(histContainer.color[iSample]);
            temp->SetFillColor(histContainer.color[iSample]);
            temp->SetFillStyle(1001);
            hStack.push_back(temp);
            leg->AddEntry(temp,histContainer.reducedNames[iSample].c_str(),"f");
        }
    }
    
    // Add data
    for( unsigned int iSample = 0; iSample < nSamples; ++iSample )
    {
        if( histContainer.isData[iSample] )
        {
            histContainer.histograms[iSample]->SetLineColor(histContainer.color[iSample]);
            histContainer.histograms[iSample]->SetMarkerColor(histContainer.color[iSample]);
            histContainer.histograms[iSample]->SetMarkerSize(1);
            histContainer.histograms[iSample]->SetMarkerStyle(20);
            histContainer.histograms[iSample]->SetLineWidth(2);
            leg->AddEntry(histContainer.histograms[iSample],histContainer.reducedNames[iSample].c_str(),"lp");
            hData = histContainer.histograms[iSample];
        }
    }

    // Set y-range
    float hMax = 0.;
    if ( hStack.size() > 0 ) 
        hMax = getMaximumIncludingErrors(hStack[hStack.size()-1]);
    if ( hData ) 
        hMax = max( getMaximumIncludingErrors(hData) , hMax );  
        
    // Draw stack
    for( int iHist = hStack.size()-1; iHist > -1; --iHist )
    {
        hStack[iHist]->GetYaxis()->SetRangeUser(0., 1.55*hMax);
        hStack[iHist]->Draw("hist same");
    }
    
    // Draw data
    if( hData ) 
    {
        hData->GetYaxis()->SetRangeUser(0., 1.55*hMax);
        hData->Draw("e same");
    }
    
    leg->Draw(); 
    for( auto* text : latexVector )
    {
        text->Draw("same");
    }
    
    c->Print((configContainer.outputDir + histContainer.containerName + "." + extension).c_str(), extension.c_str());
}

void BasePlotter::writeEfficiency(const HistogramContainer& numeratorContainer, const vector<TH1*>& denominatorHistograms, string extension, const vector<string>& binNames) const
{
    // Check numeratorContainer consistency
    if( ! numeratorContainer.check() ) 
        throw 1;
    if(  numeratorContainer.histograms.size() != denominatorHistograms.size() )
    {
        cerr << "Inconsistency found between histogramContainer \"" <<  numeratorContainer.containerName << "\" and denominatorHistograms, vectors don't have the same lenght." << endl;
        throw 1;
    }
    
    if( extension[0] == '.' )
        extension.erase(0,1);
    
    system(("mkdir -p " + configContainer.outputDir).c_str());  
    
    unsigned int nSamples = numeratorContainer.histograms.size();
    TCanvas *c = new TCanvas("Cut_eff_canv", "", 600, 600);
    TLegend* leg = nullptr;
    vector<TLatex*> latexVector;
    setCanvasAttributes( nSamples, leg, latexVector);
    
    // Calculate efficiencies
    TGraphAsymmErrors* hEff[nSamples];   
    for( unsigned int iSample = 0; iSample < nSamples; ++iSample )
    {
        hEff[iSample] = new TGraphAsymmErrors(numeratorContainer.histograms[iSample], denominatorHistograms[iSample], "cl=0.683 b(1,1) mode");
        hEff[iSample]->GetYaxis()->SetTitle("Efficiency");
        
        unsigned int binNamesSize = binNames.size();
        if( binNamesSize > 0 )
        {
            for( unsigned int iCut = 0; iCut < binNamesSize; ++iCut ) 
            {
                hEff[iSample]->GetXaxis()->SetBinLabel( hEff[iSample]->GetXaxis()->FindBin(iCut+0.5), (binNames[iCut]).c_str() );
            }
        }
    }
    
    // Set y-range
    float hMax = 0.;
    for( unsigned int iSample = 0; iSample < nSamples; ++iSample )
    {
        float tempMax = hEff[iSample]->GetMaximum();
        if (tempMax > hMax)
            hMax = tempMax;
    }        
    
    for( unsigned int iSample = 0; iSample < nSamples; ++iSample )
    {
        hEff[iSample]->SetLineColor(numeratorContainer.color[iSample]);
        hEff[iSample]->SetFillColor(numeratorContainer.color[iSample]);
        if( numeratorContainer.isData[iSample] ) 
        {
            hEff[iSample]->SetMarkerSize(1);
            hEff[iSample]->SetMarkerStyle(20);
            hEff[iSample]->SetLineWidth(2);
            leg->AddEntry(hEff[iSample],numeratorContainer.reducedNames[iSample].c_str(),"lp");
        }
        else
        {
            hEff[iSample]->SetFillStyle(1001);
            leg->AddEntry(hEff[iSample],numeratorContainer.reducedNames[iSample].c_str(),"f");
        }
    }
    
    // Draw TGraphs
    for( unsigned int iSample = 0; iSample < nSamples; ++iSample )
    {
        hEff[iSample]->GetYaxis()->SetRangeUser(0., 1.55*hMax);
        if( iSample == 0) 
            hEff[iSample]->Draw("APZ");
        else
            hEff[iSample]->Draw("PZ");
    }

    leg->Draw();
    for( auto* text : latexVector )
    {
        text->Draw("same");
    }
    
    c->Print((configContainer.outputDir + numeratorContainer.containerName + "." + extension).c_str(), extension.c_str());
}

void BasePlotter::setCanvasAttributes( unsigned int nSamples, TLegend*& leg, vector<TLatex*>& latexVector) const
{
    int nLegEntry = 2 + nSamples/2;
    leg = new TLegend (.18,.88-nLegEntry*.035,.58,.88);
    leg->SetNColumns(2);
    leg->SetBorderSize(0);
    leg->SetFillColor(0);
    leg->SetFillStyle(0);
    leg->SetTextFont(42); 
    leg->SetTextAlign(12);
    leg->SetTextSize(0.035);
    
    TLatex* TLTitle = new TLatex(.92,.86,"CMS Preliminary");
    TLTitle->SetTextSize(.04);
    TLTitle->SetTextAlign(32);
    TLTitle->SetNDC(1);
    TLTitle->SetTextFont(42);
    latexVector.push_back(TLTitle);
    
    char LumiText[50];
    sprintf (LumiText , "#sqrt{s} = %1.0f TeV, L = %4.2f fb^{-1}", 13.0 , configContainer.luminosity/1000.);
    TLatex* Lumi = new TLatex(.92,.81,LumiText);
    Lumi->SetTextSize(.03);
    Lumi->SetTextAlign(32);
    Lumi->SetNDC(1);
    Lumi->SetTextFont(42);    
    latexVector.push_back(Lumi);
}

float BasePlotter::getMaximumIncludingErrors(const TH1* h) const
{
    if( h )
    {
        float maxWithErrors = 0;
        for (Int_t i=1; i<=h->GetNbinsX(); i++) 
        {
            float binHeight = h->GetBinContent(i) + h->GetBinError(i);
            if (binHeight > maxWithErrors) 
                maxWithErrors = binHeight;
        }
        return maxWithErrors;
    }
    else
    {
        cerr << "Empty pointer passed to getMaximumIncludingErrors" << endl;
        throw 1;
    }
    return 0;
}